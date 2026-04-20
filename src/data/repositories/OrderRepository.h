/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/


#ifndef ORDERREPOSITORY_H
#define ORDERREPOSITORY_H

#include <vector>
#include "../../core/orders/Order.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class OrderRepository {
public:
    OrderRepository() = default;

    // Тільки робота з БД і не більше!
    bool insertOrder(const Order& order);
    bool updateStatus(int id, OrderStatus status);
    bool deleteOrder(int id);
    std::vector<Order> selectAllOrders();    
};

#endif
