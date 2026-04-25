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

bool OrderManager::createOrder(const QString& name, const QString& dev, const QString& issue, OrderStatus stat) {
    Order order{0, name, dev, issue, stat, QDateTime::currentDateTime()};
    return m_repository->insertOrder(order);
}

bool OrderManager::changeStatus(int id, OrderStatus status) {
    return m_repository->updateStatus(id, status);
}

bool OrderManager::deleteOrder(int id) {
    return m_repository->deleteOrder(id);
}

bool OrderManager::addOrder(const QString& name, const QString& device, const QString& issue) {
    if (name.trimmed().isEmpty()) {
        qWarning() << "Валідація провалена: Ім'я клієнта порожнє";
        return false;
    }
    if (device.trimmed().isEmpty()) {
        qWarning() << "Валідація провалена: Пристрій не вказано";
        return false;
    }
    QString finalIssue = issue.trimmed().isEmpty() ? "Діагностика" : issue;
    return createOrder(name, device, finalIssue, OrderStatus::Created);
}

std::vector<Order> OrderManager::getAllOrders() {
    return m_repository->selectAllOrders();
}
