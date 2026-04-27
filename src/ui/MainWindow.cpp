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

  m_orderManager = std::make_unique<OrderManager>();

  setupUi();
  reloadOrders();
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
}

void MainWindow::reloadOrders(const QString &filter) {
  auto allOrders = m_orderManager->getAllOrders();
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
    if (m_orderManager->addOrder(dialog.getClientName(), dialog.getDevice(), dialog.getIssue())) {
      reloadOrders();
      QMessageBox::information(this, "Успіх", "Замовлення успішно додано!");
    } else {
      QMessageBox::warning(this, "Помилка валідації", "Будь ласка, заповніть обов'язкові поля (Клієнт та Пристрій).");
    }
  }
}

void MainWindow::onDeleteOrderClicked(int id) {
  QMessageBox::StandardButton res = QMessageBox::question(this, "Видалення",
                                                          "Ви впевнені, що хочете видалити замовдення №" + QString::number(id) + "?",
                                                          QMessageBox::Yes | QMessageBox::No);
  if (res == QMessageBox::Yes) {
    if (!m_orderManager->deleteOrder(id)) {
      QMessageBox::critical(this, "Помилка видалення",
                            "База даних відхилила запит на видалення.");
    }
    reloadOrders();
  }
}

void MainWindow::onSearchTextChanged(const QString &text) {
  reloadOrders(text);
}

MainWindow::~MainWindow() {}
