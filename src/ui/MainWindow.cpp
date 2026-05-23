/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "MainWindow.h"
#include <QHeaderView>
#include <QPushButton>
#include "../core/orders/Order.h"
#include <QMessageBox>
#include <QTimer>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("veresy");
  resize(800, 600);

  m_orderManager = std::make_unique<OrderManager>(this);

  setupUi();
  m_orderManager->reloadFromRepository();
}

void MainWindow::setupUi() {
  auto *centralWidget = new QWidget(this);
  auto *layout = new QVBoxLayout(centralWidget);
  auto *topLayout = new QHBoxLayout();

  m_searchEdit = new QLineEdit(this);
  m_searchEdit->setPlaceholderText("Пошук за клієнтом або пристроєм...");
  topLayout->addWidget(m_searchEdit, 3);
  
  m_statusFilterCombo = new QComboBox(this);
  m_statusFilterCombo->addItems({
    "Всі замовлення",
    "Тільки активні",
    "Тільки завершені"
  });
  topLayout->addWidget(m_statusFilterCombo, 1);

  layout->addLayout(topLayout);

  auto *btnAdd = new QPushButton("Створити замовлення", this);
  layout->addWidget(btnAdd);
  connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddOrderClicked);

  auto *contentLayout = new QVBoxLayout();
  m_view = new OrderTableView(this);
  contentLayout->addWidget(m_view, 3);

  auto *detailGroupBox = new QGroupBox("Детальна інформація про замовлення", this);
  auto *detailLayout = new QHBoxLayout(detailGroupBox);

  auto *textDetailsLayout = new QFormLayout();
  m_lblDetailId = new QLabel("-", this);
  m_lblDetailClient = new QLabel("-", this);
  m_lblDetailDevice = new QLabel("-", this);
  m_lblDetailIssue = new QLabel("-", this);
  m_lblDetailStatus = new QLabel("-", this);
  m_lblDetailDate = new QLabel("-", this);

  textDetailsLayout->addRow("Номер замовлення:", m_lblDetailId);
  textDetailsLayout->addRow("Клієнт:", m_lblDetailClient);
  textDetailsLayout->addRow("Пристрій:", m_lblDetailDevice);
  textDetailsLayout->addRow("Опис проблеми:", m_lblDetailIssue);
  textDetailsLayout->addRow("Поточний статус:", m_lblDetailStatus);
  textDetailsLayout->addRow("Дата створення:", m_lblDetailDate);

  detailLayout->addLayout(textDetailsLayout, 2);

  m_historyList = new QListWidget(this);
  m_historyList->addItem("Оберіть замовлення для перегляду історії");
  contentLayout->addWidget(m_historyList, 1);

  contentLayout->addWidget(detailGroupBox, 1);

  layout->addLayout(contentLayout);
  setCentralWidget(centralWidget);

  m_proxyModel = new QSortFilterProxyModel(this);
  m_proxyModel->setSourceModel(m_view->model());
  m_view->setModel(m_proxyModel);

  m_view->horizontalHeader()->setSortIndicatorShown(true);
  m_view->setSortingEnabled(true);
  m_view->sortByColumn(0, Qt::DescendingOrder); 
  
  QTimer::singleShot(0, this, [this]() {
    reloadOrders();
  });

  m_statusTotalLabel = new QLabel(this);
  m_statusActiveLabel = new QLabel(this);
  m_statusFilterLabel = new QLabel(this);

  auto *boardLayout = new QHBoxLayout();

  m_colCreated = new QListWidget(this);
  m_colInProgress = new QListWidget(this);
  m_colWaitingParts = new QListWidget(this);
  m_colDone = new QListWidget(this);

  boardLayout->addWidget(m_colCreated);
  boardLayout->addWidget(m_colInProgress);
  boardLayout->addWidget(m_colWaitingParts);
  boardLayout->addWidget(m_colDone);

  layout->addLayout(boardLayout);

  statusBar()->addPermanentWidget(m_statusFilterLabel, 2);
  statusBar()->addPermanentWidget(m_statusActiveLabel, 1);
  statusBar()->addPermanentWidget(m_statusTotalLabel, 1);

  connect(m_searchEdit, &QLineEdit::textChanged, this, [this](){ this->reloadOrders(); });
  connect(m_statusFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](){ this->reloadOrders(); });
  
  connect(m_view, &OrderTableView::statusChanged, this, [this](int id, int index){
    OrderStatus newStatus = static_cast<OrderStatus>(index);
    OperationResult result;

    switch (newStatus) {
      case OrderStatus::InProgress: result = m_orderManager->startRepair(id); break;
      case OrderStatus::WaitingParts: result = m_orderManager->waitForParts(id); break;
      case OrderStatus::Done: result = m_orderManager->completeRepair(id); break;
      case OrderStatus::Cancelled: result = m_orderManager->cancelRepair(id); break;
      default:
       result = OperationResult::Fail("Невідома дія");
       break;
    }

    if (!result.success) {
      QMessageBox::critical(this, "Помилка воркфлоу", result.errorMassage);
      reloadOrders();
    }
  });
  connect(m_view, &OrderTableView::deleteRequested, this, &MainWindow::onDeleteOrderClicked);

  connect(m_orderManager.get(), &OrderManager::ordersReloaded, this, [this]() { this->reloadOrders(); });

  connect(m_view, &QTableView::doubleClicked, this, &MainWindow::onRowDoubleClicked);

  QShortcut *shortcutNew = new QShortcut(QKeySequence("Ctrl+N"), this);
  connect(shortcutNew, &QShortcut::activated, this, &MainWindow::onAddOrderClicked);

  QShortcut *shortcutSearch = new QShortcut(QKeySequence("Ctrl+F"), this);
  connect(shortcutSearch, &QShortcut::activated, [this]() {
    m_searchEdit->setFocus();
    m_searchEdit->selectAll();
  });

  QShortcut *shortcutDelete = new QShortcut(QKeySequence(Qt::Key_Delete), this);
  connect(shortcutDelete, &QShortcut::activated, [this]() {
    QModelIndex proxyIndex = m_view->currentIndex();
    if (proxyIndex.isValid()) {
      QModelIndex originalIndex = m_proxyModel->mapToSource(proxyIndex);
      auto orders = m_orderManager->getOrders();
      int row = originalIndex.row();
      if (row < orders.size()) {
        this->onDeleteOrderClicked(orders[row].id);
      }
    }
  });

  QShortcut *shortcutEnter = new QShortcut(QKeySequence(Qt::Key_Return), this);
  QShortcut *shortcutEnterNum = new QShortcut(QKeySequence(Qt::Key_Enter), this);

  connect(shortcutEnter, &QShortcut::activated, this, &MainWindow::onEditCurrentOrderRequested);
  connect(shortcutEnterNum, &QShortcut::activated, this, &MainWindow::onEditCurrentOrderRequested);

  connect(m_view->selectionModel(), &QItemSelectionModel::currentChanged,
          this, &MainWindow::onOrderSelectionChanged);
}

void MainWindow::reloadOrders() {
  auto allOrders = m_orderManager->getOrders();
  std::vector<Order> filteredOrders;

  QString filterText = m_searchEdit->text();
  int filterMode = m_statusFilterCombo->currentIndex();

  bool isFilteringActive = !filterText.isEmpty() || filterMode != 0;

  int totalCount = allOrders.size();
  int activeCount = 0;

  for (const auto& order : allOrders) {
    if (order.status != OrderStatus:: Done && order.status != OrderStatus::Cancelled) {
      activeCount++;
    }
    bool matchesSearch = filterText.isEmpty() ||
                         order.clientName.contains(filterText, Qt::CaseInsensitive) ||
                         order.device.contains(filterText, Qt::CaseInsensitive);
    if (!matchesSearch) continue;
    bool matchesStatus = true;
    if (filterMode == 1) {
      matchesStatus = (order.status != OrderStatus::Done && order.status != OrderStatus::Cancelled);
    }
    else if (filterMode == 2) {
      matchesStatus = (order.status == OrderStatus::Done || order.status == OrderStatus::Cancelled);
    }
    if (matchesStatus) {
      filteredOrders.push_back(order);
    }
  }
  m_view->updateData(filteredOrders, isFilteringActive);

  m_statusTotalLabel->setText(QString("Всього замовлент: %1").arg(totalCount));
  m_statusActiveLabel->setText(QString("Активні ремонти: %1").arg(activeCount));

  QString filterInfo = "Фільтр: " + m_statusFilterCombo->currentText();
  if (!filterText.isEmpty()) {
    filterInfo += QString(" + Пошук: \"%1\"").arg(filterText);
  }
  m_statusFilterLabel->setText(filterInfo);
  updateWorkflowBoard();
}

void MainWindow::onAddOrderClicked() {
  OrderDialog dialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    OperationResult result = m_orderManager->addOrder(dialog.getClientName(), dialog.getDevice(), dialog.getIssue());
    if (result.success) {
      QMessageBox::information(this, "Успіх", "Замовлення успішно додано!");
      m_searchEdit->setFocus();
    } else {
      QMessageBox::warning(this, "Помилка", result.errorMassage);
    }
  }
}

void MainWindow::onDeleteOrderClicked(int id) {
  QMessageBox::StandardButton res = QMessageBox::question(this, "Видалення",
                                                          "Ви впевнені, що хочете видалити замовдення №" + QString::number(id) + "?",
                                                          QMessageBox::Yes | QMessageBox::No);
  if (res == QMessageBox::Yes) {
    OperationResult result = m_orderManager->deleteOrder(id);
    if (!result.success) {
      QMessageBox::critical(this, "Помилка видалення", result.errorMassage);
    }
  }
}

void MainWindow::onRowDoubleClicked(const QModelIndex &proxyIndex) {
  if (!proxyIndex.isValid()) return;
  QModelIndex originalIndex = m_proxyModel->mapToSource(proxyIndex);
  int row = originalIndex.row();
  auto orders = m_orderManager->getOrders();
  if (row >= orders.size()) return;
  const Order& targetOrder = orders[row];
  OrderDialog dialog(this);
  dialog.setOrderData(targetOrder);

  if (dialog.exec() == QDialog::Accepted) {
    Order updatedOrder;
    updatedOrder.id = targetOrder.id;
    updatedOrder.clientName = dialog.getClientName();
    updatedOrder.device = dialog.getDevice();
    updatedOrder.issue = dialog.getIssue();
    updatedOrder.status = targetOrder.status;
    updatedOrder.createdAt = targetOrder.createdAt;
    OperationResult result = m_orderManager->updateOrder(updatedOrder);
    if (result.success) {
      QMessageBox::information(this, "Успіх", "Замовдкння успішно оновлено!");
    } else {
      QMessageBox::warning(this, "Помилка валідації", "Не вдалося оновити. Перевірте обов'язкові поля. ");
    }
  }
}

void MainWindow::onEditCurrentOrderRequested(){
  if (m_searchEdit->hasFocus()) return;
  QModelIndex currentIndex = m_view->currentIndex();
  if (!currentIndex.isValid()) return;
  this->onRowDoubleClicked(currentIndex);
}

void MainWindow::onOrderSelectionChanged(const QModelIndex &currentProxyIndex, const QModelIndex &) {
  m_historyList->clear();

  if (!currentProxyIndex.isValid()) {
    m_lblDetailId->setText("-");
    m_lblDetailClient->setText("-");
    m_lblDetailDevice->setText("-");
    m_lblDetailIssue->setText("-");
    m_lblDetailStatus->setText("-");
    m_lblDetailDate->setText("-");
    return;
  }

  QModelIndex originalIndex = m_proxyModel->mapToSource(currentProxyIndex);
  int row = originalIndex.row();

  auto orders = m_orderManager->getOrders();
  if (row >= orders.size()) return;

  const Order& order = orders[row];

  m_lblDetailId->setText(QString("#%1").arg(order.id));
  m_lblDetailClient->setText(order.clientName);
  m_lblDetailDevice->setText(order.device);
  m_lblDetailIssue->setText(order.issue);
  m_lblDetailStatus->setText(statusToString(order.status));
  m_lblDetailDate->setText(order.createdAt.toString("dd.MM.yyyy HH:mm:ss"));

  auto history = m_orderManager->getOrderHistory(order.id);

  for (const auto& record : history) {
    QString timeStr = record.timestamp.toString("HH:mm");
    QString logLine = QString("[%1] %2 > %3")
                      .arg(timeStr)
                      .arg(statusToString(record.oldStatus))
                      .arg(statusToString(record.newStatus));
    if (!record.comment.isEmpty()) {
      logLine += " (" + record.comment + ")";
    }
    m_historyList->addItem(logLine);
  }
}

void MainWindow::updateWorkflowBoard() {
  m_colCreated->clear();
  m_colInProgress->clear();
  m_colWaitingParts->clear();
  m_colDone->clear();

  auto orders = m_orderManager->getOrders();

  for (const auto& order : orders) {
    QString itemText = QString("#%1 | %2\nПроблема: %3")
                       .arg(order.id)
                       .arg(order.device)
                       .arg(order.issue);
    switch (order.status) {
      case OrderStatus::Created:
       m_colCreated->addItem(itemText);
       break;
      case OrderStatus::InProgress:
       m_colInProgress->addItem(itemText);
       break;
      case OrderStatus::WaitingParts:
       m_colWaitingParts->addItem(itemText);
       break;
      case OrderStatus::Done:
       m_colDone->addItem(itemText);
       break;
      default:
       break;
    }
  }
}

MainWindow::~MainWindow() {}
