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
#include <QRegularExpression>

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
    for (auto& order : m_orderCache) {
        if (order.id == id) {
            order.status = status;
            return updateOrder(order);
        }
    }
    return false;
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
    QString cleanName = sanitizeString(name);
    QString cleanDevice = sanitizeString(dev);
    QString cleanIssue = sanitizeString(iss);

    if (cleanName.isEmpty() || cleanDevice.isEmpty()) {
        qWarning() << "Валідація провалена: порожні поля після санітизації";
        return false;
    } 
    QString finalIssue = cleanIssue.isEmpty() ? "Діагностика" : cleanIssue;
    
    bool success = createOrder(cleanName, cleanDevice, finalIssue, OrderStatus::Created);
    if (success) {
        emit orderCreated();
        reloadFromRepository();
    } 

    return success;
}

bool OrderManager::updateOrder(const Order& order) {
    Order cleanOrder = order;
    cleanOrder.clientName = sanitizeString(order.clientName);
    cleanOrder.device = sanitizeString(order.device);
    cleanOrder.issue = sanitizeString(order.issue);

    if (cleanOrder.clientName.isEmpty() || cleanOrder.device.isEmpty()) {
        qWarning() << "Валідація провалена при редагуванні замовлення" << order.id;
        return false;
    }

    bool success = m_repository->updateOrder(cleanOrder);
    if (success) {
        emit orderUpdated();
        reloadFromRepository();
    }
    return success;
}

QString OrderManager::sanitizeString(const QString& str) const {
        QString cleanStr = str.trimmed();
        cleanStr.replace(QRegularExpression("\\s+"), " ");
        return cleanStr;
}
