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

  m_searchEdit = new QLineEdit(this);
  m_searchEdit->setPlaceholderText("Пошук за клієнтом або пристроєм...");
  layout->addWidget(m_searchEdit);
  connect(m_searchEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);

  auto *btnAdd = new QPushButton("Створити замовлення", this);
  layout->addWidget(btnAdd);
  connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddOrderClicked);

  m_view = new OrderTableView(this);
  layout->addWidget(m_view);
  setCentralWidget(centralWidget);

  connect(m_view, &OrderTableView::statusChanged, this, [this](int id, int index){
    if (!m_orderManager->changeStatus(id, static_cast<OrderStatus>(index))) {
      QMessageBox::critical(this, "Помидка", "Не вдалося оновити статус.");
      reloadOrders();
    }
  });
  connect(m_view, &OrderTableView::deleteRequested, this, &MainWindow::onDeleteOrderClicked);

  connect(m_orderManager.get(), &OrderManager::ordersReloaded, this, [this]() {
    this->reloadOrders(m_searchEdit->text());
  });

  connect(m_view, &QTableView::doubleClicked, this, &MainWindow::onRowDoubleClicked);
}

void MainWindow::reloadOrders(const QString &filter) {
  auto allOrders = m_orderManager->getOrders();
  std::vector<Order> filteredOrders;

  for (const auto& order : allOrders) {
    if (filter.isEmpty() ||
    order.clientName.contains(filter, Qt::CaseInsensitive) ||
    order.device.contains(filter, Qt::CaseInsensitive)) {
    filteredOrders.push_back(order);
    }
  }

  m_view->updateData(filteredOrders);
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

void MainWindow::onSearchTextChanged(const QString &text) {
  reloadOrders(text);
}

void MainWindow::onRowDoubleClicked(const QModelIndex &index) {
  if (!index.isValid()) return;
  int row = index.row();
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
MainWindow::~MainWindow() {}
