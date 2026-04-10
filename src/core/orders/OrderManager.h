#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <vector>
#include "Order.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class OrderManager {
public:
  OrderManager() = default;

  bool createOrder(const QString& clientName, const QString& device, const QString& issue, const QString& status);

  std::vector<Order> getAllOrders();
};

#endif
