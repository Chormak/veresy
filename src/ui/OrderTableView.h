/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef ORDERTABLEVIEW_H
#define ORDERTABLEVIEW_H

#include <QTableView>
#include <vector>
#include "OrderTableModel.h"
#include "../core/orders/Order.h"
#include <QPaintEvent>

class OrderTableView : public QTableView {
  Q_OBJECT
public:
  explicit OrderTableView(QWidget *parent = nullptr);

  void updateData(const std::vector<Order>& orders, bool isSearchingOrFiltering = false);

signals:
  void statusChanged(int orderId, int statusIndex);
  void deleteRequested(int orderId);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  OrderTableModel* m_model;
  void setupAppearance();

  bool m_isEmpty = true;
  bool m_isSearchEmpty = false;
};

#endif
