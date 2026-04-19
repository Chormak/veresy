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
#include <QComboBox>
#include <QPushButton>
#include "../core/orders/Order.h"

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

    QComboBox* combo = new QComboBox();
    combo->addItems({"Created", "In Progress", "Waiting Parts", "Done", "Cancelled"});
    combo->setCurrentText(statusToString(order.status));
    combo->setProperty("orderId", order.id);
    connect(combo, &QComboBox::currentTextChanged,
            this, [this, combo](const QString& newStatusText) {
              int id = combo->property("orderId").toInt();
              this->onStatusChanged(id, newStatusText);
            });
    m_table->setCellWidget(row, 4, combo);
    m_table->setItem(row, 5, new QTableWidgetItem(order.createdAt.toString("dd.MM.yyyy HH:mm")));
  }
}

void MainWindow::onStatusChanged(int orderId, const QString& newStatusText) {
  OrderStatus newStatus = stringToStatus(newStatusText);
  if (m_orderManager->changeStatus(orderId, newStatus)) {
    qDebug() << "Статус змінено → Замовлення" << orderId << "→" << newStatusText;
  } else {
    qWarning() << "Помилка зміни статусу для замовлення" << orderId;
  }
}

void MainWindow::onAddOrderClicked() {
  OrderDialog dialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    m_orderManager->createOrder(
      dialog.getClientName(),
      dialog.getDevice(),
      dialog.getIssue(),
      OrderStatus::Created
    );
    loadOrders();
  }
}

MainWindow::~MainWindow() {}
