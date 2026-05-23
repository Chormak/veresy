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
#include "OrderTableView.h"
#include <QTableView>
#include <QVBoxLayout>
#include <memory>
#include "../core/orders/OrderManager.h"
#include "OrderDialog.h"
#include <QLineEdit>
#include "OrderTableModel.h"
#include <QShortcut>
#include <QComboBox>
#include <QSortFilterProxyModel>
#include <QLabel>
#include <QListWidget>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  void setupUi();
  void reloadOrders();

  QComboBox *m_statusFilterCombo;
  std::unique_ptr<OrderManager> m_orderManager;
  QLineEdit *m_searchEdit;
  OrderTableView *m_view;
  QSortFilterProxyModel *m_proxyModel;

  QLabel *m_statusTotalLabel;
  QLabel *m_statusActiveLabel;
  QLabel *m_statusFilterLabel;

  QListWidget *m_historyList;

  QListWidget *m_colCreated;
  QListWidget *m_colInProgress;
  QListWidget *m_colWaitingParts;
  QListWidget *m_colDone;
  void updateWorkflowBoard();

private slots:
  void onAddOrderClicked();
  void onDeleteOrderClicked(int id);
  void onRowDoubleClicked(const QModelIndex &index);
  void onEditCurrentOrderRequested();
  void onOrderSelectionChanged(const QModelIndex & current, const QModelIndex &previous);
};

#endif
