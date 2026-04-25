/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "OrderTableModel.h"

OrderTableModel::OrderTableModel(QObject *parent) : QAbstractTableModel(parent) {}

int OrderTableModel::rowCount(const QModelIndex &) const {return m_orders.size(); }
int OrderTableModel::columnCount(const QModelIndex &) const {return 7; }

void OrderTableModel::setOrders(const std::vector<Order>& order) {
  beginResetModel();
  m_orders = order;
  endResetModel();
}

QVariant OrderTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal) return QVariant();

  switch (section) {
    case 0: return "ID";
    case 1: return "Клієнт";
    case 2: return "Пристрій";
    case 3: return "Проблема";
    case 4: return "Статус";
    case 5: return "Дата";
    default: return QVariant();
  }
}

QVariant OrderTableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

  const auto& order = m_orders[index.row()];

  switch (index.column()) {
    case 0: return order.id;
    case 1: return order.clientName;
    case 2: return order.device;
    case 3: return order.issue;
    case 4: return statusToString(order.status);
    case 5: return order.createdAt.toString("dd.MM.yyyy HH:mm");
    default: return QVariant();
  }
}
