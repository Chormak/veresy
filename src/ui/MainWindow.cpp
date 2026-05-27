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
#include <QMessageBox>
#include <QTimer>
#include <QStatusBar>

#include "screens/AnalyticsScreen.h"
#include "screens/FinanceScreen.h"
#include "screens/InventoryScreen.h"

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

  m_navigationBar = new QTabBar(this);
  m_navigationBar->addTab("Замовлення");
  m_navigationBar->addTab("Склад");
  m_navigationBar->addTab("Аналітика");
  m_navigationBar->addTab("Фінанси");
  layout->addWidget(m_navigationBar);

  m_screenStack = new QStackedWidget(this);
  layout->addWidget(m_screenStack);
  setCentralWidget(centralWidget);

  auto *ordersScreenWidget = new QWidget(this);
  auto *ordersScreenLayout = new QVBoxLayout(ordersScreenWidget);
  ordersScreenLayout->setContentsMargins(0, 0, 0, 0);

  m_toolbar = new AppToolbar(this);
  ordersScreenLayout->addWidget(m_toolbar);

  m_ordersView = new OrdersView(this);
  ordersScreenLayout->addWidget(m_ordersView);

  m_screenStack->addWidget(ordersScreenWidget);

  m_screenStack->addWidget(new InventoryScreen(this));
  m_screenStack->addWidget(new AnalyticsScreen(this));
  m_screenStack->addWidget(new FinanceScreen(this));

  
  setCentralWidget(centralWidget);

  m_statusTotalLabel = new QLabel(this);
  m_statusActiveLabel = new QLabel(this);
  m_statusFilterLabel = new QLabel(this);
  statusBar()->addPermanentWidget(m_statusFilterLabel, 2);
  statusBar()->addPermanentWidget(m_statusActiveLabel, 1);
  statusBar()->addPermanentWidget(m_statusTotalLabel, 1);

  connect(m_navigationBar, &QTabBar::currentChanged, m_screenStack, &QStackedWidget::setCurrentIndex);

  connect(m_toolbar, &AppToolbar::filtersChanged, this, &MainWindow::reloadOrders);
  connect(m_orderManager.get(), &OrderManager::ordersReloaded, this, &MainWindow::reloadOrders);

  connect(m_ordersView, &OrdersView::statusChanged, this, [this](int id, int index){
    OrderStatus newStatus = static_cast<OrderStatus>(index);
    OperationResult result;
    switch (newStatus) {
          case OrderStatus::InProgress: result = m_orderManager->startRepair(id); break;
          case OrderStatus::WaitingParts: result = m_orderManager->waitForParts(id); break;
          case OrderStatus::Done: result = m_orderManager->completeRepair(id); break;
          case OrderStatus::Cancelled: result = m_orderManager->cancelRepair(id); break;
          default: result = OperationResult::Fail("Невідома дія"); break;
    }
    if (!result.success) {
          QMessageBox::critical(this, "Помилка воркфлоу", result.errorMassage);
          reloadOrders();
    }
  });

  connect(m_ordersView, &OrdersView::orderDroppedOnBoard, this, [this](int id, OrderStatus targetStatus) {
    OperationResult result;
    switch (targetStatus) {
          case OrderStatus::InProgress: result = m_orderManager->startRepair(id); break;
          case OrderStatus::WaitingParts: result = m_orderManager->waitForParts(id); break;
          case OrderStatus::Done: result = m_orderManager->completeRepair(id); break;
          case OrderStatus::Cancelled: result = m_orderManager->cancelRepair(id); break;
          default: result = OperationResult::Fail("Дія не підтримується конвеєром."); break;
    }
    if (!result.success) {
          QMessageBox::critical(this, "Помилка воркфлоу", result.errorMassage);
    }
    reloadOrders();
  });

  connect(m_toolbar, &AppToolbar::addOrderRequested, this, &MainWindow::onAddOrderClicked);
  connect(m_toolbar, &AppToolbar::filtersChanged, this, &MainWindow::reloadOrders);

  connect(m_ordersView, &OrdersView::deleteRequested, this, &MainWindow::onDeleteOrderClicked);
  connect(m_ordersView, &OrdersView::doubleClicked, this, &MainWindow::onRowDoubleClicked);

  connect(m_ordersView, &OrdersView::historyRequested, this, [this](int orderId, std::vector<HistoryRecord>& outHistory){
    outHistory = m_orderManager->getOrderHistory(orderId);
  });

  QShortcut *shortcutNew = new QShortcut(QKeySequence("Ctrl+N"), this);
  connect(shortcutNew, &QShortcut::activated, this, &MainWindow::onAddOrderClicked);

  QShortcut *shortcutSearch = new QShortcut(QKeySequence("Ctrl+F"), this);
  connect(shortcutSearch, &QShortcut::activated, this, [this]() {
    m_toolbar->focusSearch();
  });

  QShortcut *shortcutDelete = new QShortcut(QKeySequence(Qt::Key_Delete), this);
  connect(shortcutDelete, &QShortcut::activated, [this]() {
    QModelIndex proxyIndex = m_ordersView->tableView()->currentIndex();
    if (proxyIndex.isValid()) {
          QModelIndex originalIndex = m_ordersView->proxyModel()->mapToSource(proxyIndex);
          auto orders = m_orderManager->getOrders();
          int row = originalIndex.row();
          if (row < orders.size()) this->onDeleteOrderClicked(orders[row].id);
    }
  });

  QShortcut *shortcutEnter = new QShortcut(QKeySequence(Qt::Key_Return), this);
  QShortcut *shortcutEnterNum = new QShortcut(QKeySequence(Qt::Key_Enter), this);
  connect(shortcutEnter, &QShortcut::activated, this, &MainWindow::onEditCurrentOrderRequested);
  connect(shortcutEnterNum, &QShortcut::activated, this, &MainWindow::onEditCurrentOrderRequested);

  QTimer::singleShot(0, this, [this]() { reloadOrders(); });
}

void MainWindow::reloadOrders() {
  auto allOrders = m_orderManager->getOrders();

  QString searchText = m_toolbar->filterText();
  int filterMode = m_toolbar->filterMode();
  bool isFilteringActive = !searchText.isEmpty() || filterMode != 0;

  auto filteredOrders = m_orderManager->getFilteredOrders(searchText, filterMode);

  m_ordersView->updateData(filteredOrders, isFilteringActive, allOrders);

  m_statusTotalLabel->setText(QString("Всього замовлент: %1").arg(allOrders.size()));
  m_statusActiveLabel->setText(QString("Активні ремонти: %1").arg(
    m_orderManager->getFilteredOrders("", 1).size()
  ));

  QString filterName = (filterMode == 1) ? "Тільки активні" : (filterMode == 2) ? "Тільки завершені" : "Всі замовлення";
  QString filterInfo = "Фільтр: " + filterName;
  if (!searchText.isEmpty()) {
    filterInfo += QString(" + Пошук: \"%1\"").arg(searchText);
  }
  m_statusFilterLabel->setText(filterInfo);
}

void MainWindow::onAddOrderClicked() {
  OrderDialog dialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    OperationResult result = m_orderManager->addOrder(dialog.getClientName(), dialog.getDevice(), dialog.getIssue());
    if (result.success) {
      QMessageBox::information(this, "Успіх", "Замовлення успішно додано!");
      m_toolbar->focusSearch();
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
  QModelIndex originalIndex = m_ordersView->proxyModel()->mapToSource(proxyIndex);
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
  QModelIndex currentIndex = m_ordersView->tableView()->currentIndex();
  if (!currentIndex.isValid()) return;
  this->onRowDoubleClicked(currentIndex);
}


MainWindow::~MainWindow() {}
