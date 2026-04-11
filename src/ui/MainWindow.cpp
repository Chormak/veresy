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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("veresy");
  resize(800, 600);

  m_orderManager = std::make_unique<OrderManager>();

  setupUi();
  loadOrders();
}

void MainWindow::setupUi() {
  auto *centralWidget = new QWidget(this);
  auto *layout = new QVBoxLayout(centralWidget);
  
  auto *btnAdd = new QPushButton("Створити замовлення", this);
  layout->addWidget(btnAdd);
  connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddOrderClicked);

  m_table = new QTableWidget(this);
  m_table->setColumnCount(6);
  m_table->setHorizontalHeaderLabels({"ID", "Клієнт", "Пристрій", "Проблема", "Статус", "Дата"});

  m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  layout->addWidget(m_table);
  setCentralWidget(centralWidget);
}

void MainWindow::loadOrders() {
  auto orders = m_orderManager->getAllOrders();
  m_table->setRowCount(0);

  for (const auto& order : orders) {
    int row = m_table->rowCount();
    m_table->insertRow(row);

    m_table->setItem(row, 0, new QTableWidgetItem(QString::number(order.id)));
    m_table->setItem(row, 1, new QTableWidgetItem(order.clientName));
    m_table->setItem(row, 2, new QTableWidgetItem(order.device));
    m_table->setItem(row, 3, new QTableWidgetItem(order.issue));
    m_table->setItem(row, 4, new QTableWidgetItem(order.status));
    m_table->setItem(row, 5, new QTableWidgetItem(order.createdAt.toString("dd.MM.yyyy HH:mm")));
  }
}

void MainWindow::onAddOrderClicked() {
  OrderDialog dialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    m_orderManager->createOrder(
      dialog.getClientName(),
      dialog.getDevice(),
      dialog.getIssue(),
      "Нове"
    );
    loadOrders();
  }
} 

MainWindow::~MainWindow() {}
