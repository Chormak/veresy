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
    OperationResult res = m_repository->insertOrder(order);
    return res.success;
}

bool OrderManager::changeStatus(int id, OrderStatus status) {
    for (auto& order : m_orderCache) {
        if (order.id == id) {
            order.status = status;
            OperationResult res = updateOrder(order);
            return res.success;
        }
    }
    return false;
}

OperationResult OrderManager::deleteOrder(int id) {
    OperationResult ress = m_repository->deleteOrder(id);
    if (ress.success) {
        emit orderDeleted();
        reloadFromRepository();
    }
    return ress;
}

OperationResult OrderManager::addOrder(const QString& name, const QString& dev, const QString& iss) {
    QString cleanName = sanitizeString(name);
    QString cleanDevice = sanitizeString(dev);
    QString cleanIssue = sanitizeString(iss);

    if (cleanName.isEmpty() || cleanDevice.isEmpty()) {
        return OperationResult::Fail("Ім'я клієнта та назва пристрою не можуть бути порожніми.");
    } 
    QString finalIssue = cleanIssue.isEmpty() ? "Діагностика" : cleanIssue;

    OperationResult res = m_repository->insertOrder({0, cleanName, cleanDevice, finalIssue, OrderStatus::Created, QDateTime::currentDateTime()});
    if (res.success) {
        emit orderCreated();
        reloadFromRepository();
    } 

    return res;
}

OperationResult OrderManager::updateOrder(const Order& order) {
    Order cleanOrder = order;
    cleanOrder.clientName = sanitizeString(order.clientName);
    cleanOrder.device = sanitizeString(order.device);
    cleanOrder.issue = sanitizeString(order.issue);

    if (cleanOrder.clientName.isEmpty() || cleanOrder.device.isEmpty()) {
        return OperationResult::Fail("Ім'я клієнта та назва пристрою не можуть бути порожніми.");
    }

    bool success = m_repository->updateOrder(cleanOrder);
    if (success) {
        emit orderUpdated();
        reloadFromRepository();
        return OperationResult::Ok();
    }
    return OperationResult::Fail("Не вдалося оновити замовлення в базі даних");
}

QString OrderManager::sanitizeString(const QString& str) const {
        QString cleanStr = str.trimmed();
        cleanStr.replace(QRegularExpression("\\s+"), " ");
        return cleanStr;
}
