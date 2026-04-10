#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QVBoxLayout>
#include <memory>
#include "../core/orders/OrderManager.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  void setupUi();
  void loadOrders();

  QTableWidget *m_table;
  std::unique_ptr<OrderManager> m_orderManager;
};

#endif
