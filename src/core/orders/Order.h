/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef ORDER_H
#define ORDER_H
#include <QString>
#include <QDateTime>

enum class OrderStatus {
  Created,
  InProgress,
  WaitingParts,
  Done,
  Cancelled
};

inline QString statusToString(OrderStatus status) {
  switch (status) {
    case OrderStatus::Created: return "Created";
    case OrderStatus::InProgress: return "In Progress";
    case OrderStatus::WaitingParts: return "Waiting Parts";
    case OrderStatus::Done: return "Done";
    case OrderStatus::Cancelled: return "Cancelled";
    default: return "Unknown";
  }
}

inline OrderStatus stringToStatus(const QString& statusStr) {
    if (statusStr == "Created")      return OrderStatus::Created;
    if (statusStr == "In Progress")  return OrderStatus::InProgress;
    if (statusStr == "Waiting Parts") return OrderStatus::WaitingParts;
    if (statusStr == "Done")         return OrderStatus::Done;
    if (statusStr == "Cancelled")    return OrderStatus::Cancelled;
    return OrderStatus::Created;  // default
}

struct Order {
  int id;
  QString clientName;
  QString device;
  QString issue;
  OrderStatus status;
  QDateTime createdAt;
};

#endif
