/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "OrderManager.h"

OrderManager::OrderManager() {
    m_repository = std::make_unique<OrderRepository>();
}

bool OrderManager::createOrder(const QString& name, const QString& dev, const QString& iss, OrderStatus stat) {
    Order order{0, name, dev, iss, stat, QDateTime::currentDateTime()};
    return m_repository->insertOrder(order);
}

bool OrderManager::changeStatus(int id, OrderStatus status) {
    return m_repository->updateStatus(id, status);
}


std::vector<Order> OrderManager::getAllOrders() {
    return m_repository->selectAllOrders();
}
