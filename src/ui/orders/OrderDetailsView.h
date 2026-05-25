/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef ORDERDETAILSVIEW_H
#define ORDERDETAILSVIEW_H

#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <vector>
#include "../../core/orders/Order.h"

class OrderDetailsView : public QGroupBox {
  Q_OBJECT
public:
  explicit OrderDetailsView(QWidget *parent = nullptr);

  void showOrderDetails(const Order& order, const std::vector<HistoryRecord>& history);

  void clearDetails();

private:
  void setupUi();

  QLabel *m_lblDetailId;
  QLabel *m_lblDetailClient;
  QLabel *m_lblDetailDevice;
  QLabel *m_lblDetailIssue;
  QLabel *m_lblDetailStatus;
  QLabel *m_lblDetailDate;
  QListWidget *m_historyList;
};

#endif
