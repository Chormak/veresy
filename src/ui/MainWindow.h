/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "../core/orders/OrderManager.h"
#include "OrderDialog.h"
#include <QLineEdit>
#include <QShortcut>
#include <QComboBox>
#include <QLabel>
#include "orders/OrdersView.h"
#include "filters/FiltersPanel.h"
#include "common/AppToolbar.h"
#include <QStackedWidget>
#include <QTabBar>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  void setupUi();
  void reloadOrders();

  std::unique_ptr<OrderManager> m_orderManager;

  AppToolbar *m_toolbar;

  OrdersView *m_ordersView;

  QLabel *m_statusTotalLabel;
  QLabel *m_statusActiveLabel;
  QLabel *m_statusFilterLabel;

  QStackedWidget *m_screenStack;
  QTabBar *m_navigationBar;
private slots:
  void onAddOrderClicked();
  void onDeleteOrderClicked(int id);
  void onRowDoubleClicked(const QModelIndex &proxyIndex);
  void onEditCurrentOrderRequested();
};

#endif
