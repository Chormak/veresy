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

bool OrderManager::createOrder(const QString& clientName, const QString& device, const QString& issue, OrderStatus status) {
  QSqlQuery query;
  query.prepare("INSERT INTO orders (client_name, device, issue, status) "
                "VALUES (:name, :device, :issue, :status)");

  query.bindValue(":name", clientName);
  query.bindValue(":device", device);
  query.bindValue(":issue", issue);
  query.bindValue(":status", statusToString(status));
  return query.exec();
}

inline OrderStatus stringToStatus(const QString& statusStr) {
  if (statusStr == "Created") return OrderStatus::Created;
  if (statusStr == "In Progress") return OrderStatus::InProgress;
  if (statusStr == "Waiting Parts") return OrderStatus::WaitingParts;
  if (statusStr == "Done") return OrderStatus::Done;
  if (statusStr == "Cancelled") return OrderStatus::Cancelled;
  return OrderStatus::Created;
}

std::vector<Order> OrderManager::getAllOrders() {
  std::vector<Order> orders;
  QSqlQuery query("SELECT id, client_name, device, issue, status, created_at FROM orders");

  while (query.next()) {
    Order order;
    order.id = query.value(0).toInt();
    order.clientName = query.value(1).toString();
    order.device = query.value(2).toString();
    order.issue = query.value(3).toString();
    order.status = stringToStatus(query.value(4).toString()); 
    order.createdAt = query.value(5).toDateTime();

    orders.push_back(order);
  }
  return orders;
}
