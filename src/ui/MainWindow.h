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
#include <QTableWidget>
#include <QVBoxLayout>
#include <memory>
#include "../core/orders/OrderManager.h"
#include "OrderDialog.h"
#include <QLineEdit>
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  void setupUi();
  void reloadOrders(const QString &filter = "");

  QTableWidget *m_table;
  std::unique_ptr<OrderManager> m_orderManager;
  QLineEdit *m_searchEdit;

private slots:
  void onAddOrderClicked();
  void onStatusChanged(int orderId, const QString& newStatusText);
  void onDeleteOrderClicked();
  void onSearchTextChanged(const QString &text);
};

#endif
