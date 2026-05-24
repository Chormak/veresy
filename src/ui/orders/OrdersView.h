/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef ORDERSVIEW_H
#define ORDERSVIEW_H

#include <QWidget>
#include <QListWidget>
#include <QGroupBox>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSortFilterProxyModel>
#include "../OrderTableView.h"
#include "../../core/orders/Order.h"

class OrdersView : public QWidget {
  Q_OBJECT
public:
  explicit OrdersView(QWidget *parent = nullptr);

  void updateData(const std::vector<Order>& filteredOrders, bool isFilteringActive, const std::vector<Order>& allOrders);
  
  OrderTableView* tableView() const { return m_view; }
  QSortFilterProxyModel* proxyModel() const { return m_proxyModel; }

signals:
  void statusChanged(int id, int index);
  void deleteRequested(int id);
  void doubleClicked(const QModelIndex &index);

private slots:
  void onOrderSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

private:
  void setupUi();
  void updateWorkflowBoard(const std::vector<Order>& allOrders);

  OrderTableView *m_view;
  QSortFilterProxyModel *m_proxyModel;

  QLabel *m_lblDetailId;
  QLabel *m_lblDetailClient;
  QLabel *m_lblDetailDevice;
  QLabel *m_lblDetailIssue;
  QLabel *m_lblDetailStatus;
  QLabel *m_lblDetailDate;
  QListWidget *m_historyList;

  QListWidget *m_colCreated;
  QListWidget *m_colInProgress;
  QListWidget *m_colWaitingParts;
  QListWidget *m_colDone;

  std::vector<Order> m_currentFilteredOrders;

signals:
  void historyRequested(int orderId, std::vector<HistoryRecord>& outHistory);
};

#endif
