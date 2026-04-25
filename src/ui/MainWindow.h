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
#include <QTableView>
#include <QVBoxLayout>
#include <memory>
#include "../core/orders/OrderManager.h"
#include "OrderDialog.h"
#include <QLineEdit>
#include "OrderTableModel.h"
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  void setupUi();
  void reloadOrders(const QString &filter = "");

  std::unique_ptr<OrderManager> m_orderManager;
  QLineEdit *m_searchEdit;
  QTableView *m_view;
  OrderTableModel *m_model;

private slots:
  void onAddOrderClicked();
  void onStatusChanged(int orderId, const QString& newStatusText);
  void onDeleteOrderClicked();
  void onSearchTextChanged(const QString &text);
};

#endif
