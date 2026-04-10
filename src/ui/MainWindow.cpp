#include "MainWindow.h"
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("veresy");
  resize(800, 600);

  m_orderManager = std::make_unique<OrderManager>();

  setupUi();
  loadOrders();
}

void MainWindow::setupUi() {
  auto *centralWidget = new QWidget(this);
  auto *layout = new QVBoxLayout(centralWidget);

  m_table = new QTableWidget(this);
  m_table->setColumnCount(6);
  m_table->setHorizontalHeaderLabels({"ID", "Клієнт", "Пристрій", "Проблема", "Статус", "Дата"});

  m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  layout->addWidget(m_table);
  setCentralWidget(centralWidget);
}

void MainWindow::loadOrders() {
  auto orders = m_orderManager->getAllOrders();
  m_table->setRowCount(0);

  for (const auto& order : orders) {
    int row = m_table->rowCount();
    m_table->insertRow(row);

    m_table->setItem(row, 0, new QTableWidgetItem(QString::number(order.id)));
    m_table->setItem(row, 1, new QTableWidgetItem(order.clientName));
    m_table->setItem(row, 2, new QTableWidgetItem(order.device));
    m_table->setItem(row, 3, new QTableWidgetItem(order.issue));
    m_table->setItem(row, 4, new QTableWidgetItem(order.status));
    m_table->setItem(row, 5, new QTableWidgetItem(order.createdAt.toString("dd.MM.yyyy HH:mm")));
  }
}

MainWindow::~MainWindow() {}
