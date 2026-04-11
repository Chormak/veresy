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

struct Order {
  int id;
  QString clientName;
  QString device;
  QString issue;
  QString status;
  QDateTime createdAt;
};

#endif
