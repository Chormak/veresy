/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/


#include "OrderRepository.h"

OperationResult OrderRepository::insertOrder(const Order& order) {
    QSqlQuery query;
    query.prepare("INSERT INTO orders (client_name, device, issue, status) "
                  "VALUES (:name, :device, :issue, :status)");
    
    query.bindValue(":name", order.clientName);
    query.bindValue(":device", order.device);
    query.bindValue(":issue", order.issue);
    query.bindValue(":status", static_cast<int>(order.status));

    if (!query.exec()) {
        qCritical() << "SQL Error (insert):" << query.lastError().text();
        return OperationResult::Fail("Помилка бази даних: " + query.lastError().text());
    }
    return OperationResult::Ok();
}

OperationResult OrderRepository::updateStatus(int id, OrderStatus status) {
    QSqlQuery query;
    query.prepare("UPDATE orders SET status = :status WHERE id = :id");
    query.bindValue(":status", static_cast<int>(status));
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        qCritical() << "SQL Error (update):" << query.lastError().text();
        return OperationResult::Fail("Помилка бази даних: " + query.lastError().text());
    }
    return OperationResult::Ok();
}

OperationResult OrderRepository::deleteOrder(int id) {
    if (id <= 0)
        return OperationResult::Fail("Помилка номера клієнта: ");
    QSqlQuery query;
    query.prepare("DELETE FROM orders WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qCritical() << "SQL Error (delete):" << query.lastError().text();
        return OperationResult::Fail("Помилка бази даних: " + query.lastError().text());
    }
    return OperationResult::Ok();
}

std::vector<Order> OrderRepository::selectAllOrders() {
    std::vector<Order> orders;
    QSqlQuery query("SELECT id, client_name, device, issue, status, created_at "
                    "FROM orders ORDER BY created_at DESC");

    while (query.next()) {
        Order order;
        order.id = query.value(0).toInt();
        order.clientName = query.value(1).toString();
        order.device = query.value(2).toString();
        order.issue = query.value(3).toString();
        order.status = static_cast<OrderStatus>(query.value(4).toInt());
        order.createdAt = query.value(5).toDateTime();
        orders.push_back(order);
    }
    return orders;
}

bool OrderRepository::updateOrder(const Order& order) {
    if (order.id <= 0) return false;

    QSqlQuery query;
    query.prepare("UPDATE orders SET client_name = :name, device = :device, "
                  "issue = :issue, status = :status WHERE id = :id");

    query.bindValue(":name", order.clientName);
    query.bindValue(":device", order.device);
    query.bindValue(":issue", order.issue);
    query.bindValue(":status", static_cast<int>(order.status));
    query.bindValue(":id", order.id);

    if (!query.exec()) {
        qCritical() << "SQL Error (Update Order):" << query.lastError().text();
        return false;
    }
    return true;
}

OperationResult OrderRepository::logStatusChange(int orderId, OrderStatus oldStatus, OrderStatus newStatus, const QString& comment, const QString& performedBy) {
    QSqlQuery query;
    query.prepare("INSERT INTO order_history (order_id, old_status, new_status, comment, performed_by) "
                  "VALUES (:order_id, :old_status, :new_status, :comment, :performed_by)");

    query.bindValue(":order_id", orderId);
    if (oldStatus == newStatus && oldStatus == OrderStatus::Created) {
        query.bindValue(":old_status", QVariant());
    } else {
        query.bindValue(":old_status", static_cast<int>(oldStatus));
    }
    query.bindValue(":new_status", static_cast<int>(newStatus));
    query.bindValue(":comment", comment);
    query.bindValue(":performed_by", performedBy);

    if (!query.exec()) {
        qCritical() << "SQL Error (logStatusChange):" << query.lastError().text();
        return OperationResult::Fail("Не вдалося записати історію змін: " + query.lastError().text());
    }
    return OperationResult::Ok();
}

std::vector<HistoryRecord> OrderRepository::selectOrderHistory(int orderId) {
    std::vector<HistoryRecord> history;
    QSqlQuery query;
    query.prepare("SELECT id, order_id, old_status, new_status, timestamp, comment, performed_by "
                  "FROM order_history WHERE order_id = :order_id ORDER BY timestamp ASC");
    query.bindValue(":order_id", orderId);

    if (query.exec()) {
        while (query.next()) {
            HistoryRecord record;
            record.id = query.value(0).toInt();
            record.orderId = query.value(1).toInt();
            record.oldStatus = query.value(2).isNull() ? OrderStatus::Created : static_cast<OrderStatus>(query.value(2).toInt());
            record.newStatus = static_cast<OrderStatus>(query.value(3).toInt());
            record.timestamp = query.value(4).toDateTime();
            record.comment = query.value(5).toString();
            record.performed_by = query.value(6).toString();
            history.push_back(record);
        }
    } else {
        qCritical() << "SQL Error (selectHistory):" << query.lastError().text();
    }
    return history;
}
