/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef ORDERTABLEMODEL_H
#define ORDERTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "../core/orders/Order.h"

class OrderTableModel : public QAbstractTableModel {
  Q_OBJECT

public:
  explicit OrderTableModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  void setOrders(const std::vector<Order>& orders);

private:
  std::vector<Order> m_orders;
};

#endif
