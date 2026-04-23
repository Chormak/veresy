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

  m_table = new QTableWidget(this);
  m_table->setColumnCount(7);
  m_table->setHorizontalHeaderLabels({"ID", "Клієнт", "Пристрій", "Проблема", "Статус", "Дата", "Дія"});

  m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  layout->addWidget(m_table);
  setCentralWidget(centralWidget);
}

void MainWindow::reloadOrders(const QString &filter) {
  auto orders = m_orderManager->getAllOrders();
  m_table->setRowCount(0);

  for (const auto& order : orders) {

    if (!filter.isEmpty()) {
      bool matchesClient = order.clientName.contains(filter, Qt::CaseInsensitive);
      bool matchesDevice = order.device.contains(filter, Qt::CaseInsensitive);
      if (!matchesClient && !matchesDevice) {
        continue;
      }
    }

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

    QPushButton* btnDelete = new QPushButton("Видалити", this);
    btnDelete->setProperty("orderID", order.id);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteOrderClicked);
    m_table->setCellWidget(row, 6, btnDelete);
  }
}

void MainWindow::onStatusChanged(int orderId, const QString& newStatusText) {
  OrderStatus newStatus = stringToStatus(newStatusText);
  if (m_orderManager->changeStatus(orderId, newStatus)) {
    qDebug() << "Статус змінено → Замовлення" << orderId << "→" << newStatusText;
    reloadOrders();
  } else {
    qWarning() << "Помилка зміни статусу для замовлення" << orderId;
  }
}

void MainWindow::onAddOrderClicked() {
  OrderDialog dialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    bool success = m_orderManager->createOrder(
      dialog.getClientName(),
      dialog.getDevice(),
      dialog.getIssue(),
      OrderStatus::Created
    );
    if (success) {
      reloadOrders();
      QMessageBox::information(this, "Успіх", "Замовлення успішно додано!");
    } else {
      QMessageBox::information(this, "Помидка", "Не вдалося зберегти замовлення в базі даних.");
    }
  }
}

void MainWindow::onDeleteOrderClicked() {
  QPushButton* btn = qobject_cast<QPushButton*>(sender());
  if (!btn) return;
  int id = btn->property("orderID").toInt();
  QMessageBox::StandardButton res = QMessageBox::question(this, "Видалення",
                                                          "Ви впевнені, що хочете видалити замовдення №" + QString::number(id) + "?",
                                                          QMessageBox::Yes | QMessageBox::No);
  if (res == QMessageBox::Yes) {
    if (m_orderManager->deleteOrder(id)) {
      reloadOrders();
    }
  }
}

void MainWindow::onSearchTextChanged(const QString &text) {
  reloadOrders(text);
}

MainWindow::~MainWindow() {}
