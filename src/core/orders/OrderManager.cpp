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

OrderManager::OrderManager(QObject *parent) : QObject(parent) {
    m_repository = std::make_unique<OrderRepository>();
}
void OrderManager::reloadFromRepository() {
    m_orderCache = m_repository->selectAllOrders();

    emit ordersReloaded();
}
std::vector<Order> OrderManager::getOrders() const {
    return m_orderCache;
}

bool OrderManager::createOrder(const QString& name, const QString& dev, const QString& iss, OrderStatus stat) {
    Order order{0, name, dev, iss, stat, QDateTime::currentDateTime()};
    return m_repository->insertOrder(order);
}

bool OrderManager::changeStatus(int id, OrderStatus status) {
    bool success = m_repository->updateStatus(id, status);
    if (success) {
        emit orderUpdated();
        reloadFromRepository();
    }
    return success;
}

bool OrderManager::deleteOrder(int id) {
    bool success = m_repository->deleteOrder(id);
    if (success) {
        emit orderDeleted();
        reloadFromRepository();
    }
    return success;
}

bool OrderManager::addOrder(const QString& name, const QString& dev, const QString& iss) {
    if (name.trimmed().isEmpty() || dev.trimmed().isEmpty()) return false;
    QString finalIssue = iss.trimmed().isEmpty() ? "Діагностика" : iss;

    bool success = createOrder(name, dev, finalIssue, OrderStatus::Created);
    if (success) {
        emit orderCreated();
        reloadFromRepository();
    }
    return success;
}
