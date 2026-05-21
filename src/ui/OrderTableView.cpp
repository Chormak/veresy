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

OrderTableView::OrderTableView(QWidget *parent) : QTableView(parent) {
  m_model = new OrderTableModel(this);
  setModel(m_model);
  setupAppearance();
}

void OrderTableView::setupAppearance() {
  horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  setSelectionBehavior(QAbstractItemView::SelectRows);
}

void OrderTableView::updateData(const std::vector<Order>& orders) {
  m_model->setOrders(orders);

  for (int row = 0; row < m_model->rowCount({}); ++row) {
    setIndexWidget(m_model->index(row, 4), nullptr);
    setIndexWidget(m_model->index(row, 6), nullptr);
  }

  QAbstractItemModel* currentModel = model();

  for (int i = 0; i < orders.size(); ++i) {
    const auto& order = orders[i];

    QComboBox* combo = new QComboBox();
    combo->addItems({"Created", "In Progress", "Waiting Parts", "Done", "Cancelled"});
    combo->setCurrentIndex(static_cast<int>(order.status));
    combo->setProperty("orderId", order.id);
    
    QColor statusColor = getStatusColor(order.status);

    QPalette pal = combo->palette();
    pal.setColor(QPalette::Text, statusColor);
    pal.setColor(QPalette::WindowText, statusColor);
    combo->setStyleSheet(QString("QComboBox { color: %1; font-weight: bold; }").arg(statusColor.name()));
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
