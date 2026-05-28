/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "../../core/users/User.h"
#include "../../core/orders/Order.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class UserRepository {
public:
    UserRepository() = default;

    User getByUsername(const QString& username);

    OperationResult insertUser(const User& user);
};

#endif
