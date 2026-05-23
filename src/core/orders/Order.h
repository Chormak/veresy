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
#include <QColor>
#include <vector>

enum class OrderStatus {
  Created = 0,
  InProgress = 1,
  WaitingParts = 2,
  Done = 3,
  Cancelled = 4
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
    return OrderStatus::Created;
}

inline bool isValidStatusTransition(OrderStatus from, OrderStatus to) {
  if (from == to) return true;

  switch (from) {
    case OrderStatus::Created:
    return (to == OrderStatus::InProgress || to == OrderStatus::Cancelled);

    case OrderStatus::InProgress:
    return (to == OrderStatus::WaitingParts || to == OrderStatus::Done || to == OrderStatus::Cancelled);

    case OrderStatus::WaitingParts:
    return (to == OrderStatus::InProgress || to == OrderStatus::Cancelled);

    case OrderStatus::Done:
    return false;

    case OrderStatus::Cancelled:
    return false;

    default:
    return false;
  }
}

inline QColor getStatusColor(OrderStatus status) {
  switch (status) {
    case OrderStatus::Created:
     return QColor("#007BFF");
    case OrderStatus::InProgress:
     return QColor("#FFC107");
    case OrderStatus::WaitingParts:
     return QColor("#FD7E14");
    case OrderStatus::Done:
     return QColor("#28A745");
    case OrderStatus::Cancelled:
     return QColor("#6C757D");
    default:
     return QColor("#212529");
  }
}

inline std::vector<OrderStatus> getNextAllowedStatuses(OrderStatus current) {
  std::vector<OrderStatus> allowed;

  switch (current) {
  case OrderStatus::Created:
   allowed.push_back(OrderStatus::InProgress);
   allowed.push_back(OrderStatus::Cancelled);
   break;
  case OrderStatus::InProgress:
   allowed.push_back(OrderStatus::WaitingParts);
   allowed.push_back(OrderStatus::Done);
   allowed.push_back(OrderStatus::Cancelled);
   break;
  case OrderStatus::WaitingParts:
   allowed.push_back(OrderStatus::InProgress);
   allowed.push_back(OrderStatus::Cancelled);
   break;
  case OrderStatus::Done:
  case OrderStatus::Cancelled:
   break;
  }
  return allowed;
}

struct Order {
  int id;
  QString clientName;
  QString device;
  QString issue;
  OrderStatus status;
  QDateTime createdAt;
};

struct OperationResult {
  bool success;
  QString errorMassage;
  static OperationResult Ok() { return {true, ""};}
  static OperationResult Fail(const QString& message) { return {false, message};}
};

struct HistoryRecord {
  int id;
  int orderId;
  OrderStatus oldStatus;
  OrderStatus newStatus;
  QDateTime timestamp;
  QString comment;
  QString performed_by;
};

#endif
