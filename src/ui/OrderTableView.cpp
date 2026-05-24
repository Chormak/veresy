/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "OrderTableView.h"
#include <QHeaderView>
#include <QComboBox>
#include <QPushButton>
#include <QPainter>

OrderTableView::OrderTableView(QWidget *parent) : QTableView(parent) {
  m_model = new OrderTableModel(this);
  setModel(m_model);
  setupAppearance();
}

void OrderTableView::setupAppearance() {
  horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  setSelectionBehavior(QAbstractItemView::SelectRows);
}

void OrderTableView::updateData(const std::vector<Order>& orders, bool isSearchingOrFiltering) {
  m_isEmpty = orders.empty();
  m_isSearchEmpty = m_isEmpty && isSearchingOrFiltering;

  m_model->setOrders(orders);

  for (int row = 0; row < m_model->rowCount({}); ++row) {
    setIndexWidget(m_model->index(row, 4), nullptr);
    setIndexWidget(m_model->index(row, 6), nullptr);
  }

  QAbstractItemModel* currentModel = model();

  for (int i = 0; i < orders.size(); ++i) {
    const auto& order = orders[i];

    QComboBox* combo = new QComboBox();
    combo->addItem(statusToIcon(OrderStatus::Created) + " Created");
    combo->addItem(statusToIcon(OrderStatus::InProgress) + " In Progress");
    combo->addItem(statusToIcon(OrderStatus::WaitingParts) + " Waiting Parts");
    combo->addItem(statusToIcon(OrderStatus::Done) + " Done");
    combo->addItem(statusToIcon(OrderStatus::Cancelled) + " Cancelled");

    combo->setCurrentIndex(static_cast<int>(order.status));
    combo->setProperty("orderId", order.id);

    combo->setItemData(2, getStatusColor(OrderStatus::WaitingParts), Qt::ForegroundRole);
    
    QColor statusColor = getStatusColor(order.status);
    if (order.status == OrderStatus::WaitingParts) {
      combo->setStyleSheet(QString(
        "QComboBox { color: %1; font-weight: bold; border: 1px solid %1; background-color: #FFF5F5; }"
      ).arg(statusColor.name()));
    } else {
      combo->setStyleSheet(QString("QComboBox { color: %1; }").arg(statusColor.name()));
    }

    connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this, combo](int index){
      int id = combo->property("orderId").toInt();
      emit statusChanged(id, index);
    });

    QModelIndex proxyIdx = currentModel->index(i, 4);
    setIndexWidget(proxyIdx, combo);

    QPushButton* btnDelete = new QPushButton("Видалити", this);
    btnDelete->setProperty("orderId", order.id);
    connect(btnDelete, &QPushButton::clicked, [this, id = order.id](){
      emit deleteRequested(id);
    });
    setIndexWidget(currentModel->index(i, 6), btnDelete);
  }
}

void OrderTableView::paintEvent(QPaintEvent *event) {
  QTableView::paintEvent(event);

  if (!m_isEmpty) return;

  QPainter painter(viewport());
  painter.setPen(QColor("#6C757D"));

  QFont font = painter.font();
  font.setPointSize(12);
  font.setBold(true);
  painter.setFont(font);

  QString message;
  if (m_isSearchEmpty) {
    message = "Нічого не знайдено за вашим запитом";
  } else {
    message = "Список замовлень порожній. Створіть перше замовлення!";
  }
  painter.drawText(viewport()->rect(), Qt::AlignCenter, message);
}
