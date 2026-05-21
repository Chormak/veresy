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

#include <QObject>
#include <vector>
#include "Order.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "../../data/repositories/OrderRepository.h"

class OrderManager : public QObject {
  Q_OBJECT 
public:
  explicit OrderManager(QObject *parent = nullptr);

  OperationResult addOrder(const QString& name, const QString& dev, const QString& iss);
  bool changeStatus(int id, OrderStatus status);
  OperationResult deleteOrder(int id);
  void reloadFromRepository();
  std::vector<Order> getOrders() const;
  OperationResult updateOrder(const Order& order);
  OperationResult moveOrderToStatus(int id, OrderStatus targetStatus);
  std::vector<OrderStatus> getAllowedActionsForOrder(int id) const;

  signals:
  void orderCreated();
  void orderUpdated();
  void orderDeleted();
  void ordersReloaded();

private:
  bool createOrder(const QString& name, const QString& dev, const QString& iss, OrderStatus stat);
  std::vector<Order> m_orderCache;
  std::unique_ptr<OrderRepository> m_repository;
  QString sanitizeString(const QString& str) const;
};

#endif
