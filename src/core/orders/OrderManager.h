/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

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

  bool createOrder(const QString& clientName, const QString& device, const QString& issue, OrderStatus status);

  std::vector<Order> getAllOrders();
};

#endif
