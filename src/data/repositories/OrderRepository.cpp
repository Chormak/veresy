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

bool OrderRepository::insertOrder(const Order& order) {
    QSqlQuery query;
    query.prepare("INSERT INTO orders (client_name, device, issue, status) "
                  "VALUES (:name, :device, :issue, :status)");
    
    query.bindValue(":name", order.clientName);
    query.bindValue(":device", order.device);
    query.bindValue(":issue", order.issue);
    query.bindValue(":status", static_cast<int>(order.status));

    return query.exec();
}

bool OrderRepository::updateStatus(int id, OrderStatus status) {
    QSqlQuery query;
    query.prepare("UPDATE orders SET status = :status WHERE id = :id");
    query.bindValue(":status", static_cast<int>(status));
    query.bindValue(":id", id);
    return query.exec();
}

bool OrderRepository::deleteOrder(int id) {
    if (id <= 0)
        return false;
    QSqlQuery query;
    query.prepare("DELETE FROM orders WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec())
        return true;
    qWarning() << "Не вдалося видалити замовлення:" << query.lastError().text();
    return false;
}

std::vector<Order> OrderRepository::selectAllOrders() {
    std::vector<Order> orders;
    QSqlQuery query("SELECT id, client_name, device, issue, status, created_at FROM orders");

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
