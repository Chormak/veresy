/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "UserRepository.h"

User UserRepository::getByUsername(const QString& username) {
    User user;
    QSqlQuery query;
    query.prepare("SELECT id, username, display_name, password_hash, role, created_at, is_active "
                  "FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        user.id = query.value(0).toInt();
        user.username = query.value(1).toString();
        user.displayName = query.value(2).toString();
        user.passwordHash = query.value(3).toString();
        user.role = query.value(4).toString();
        user.createdAt = query.value(5).toDateTime();
        user.isActive = query.value(6).toInt() == 1;
    }
    return user;
}

OperationResult UserRepository::insertUser(const User& user) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, display_name, password_hash, role, is_active) "
                  "VALUES (:username, :display_name, :hash, :role, :is_active)");
    query.bindValue(":username", user.username);
    query.bindValue(":display_name", user.displayName);
    query.bindValue(":hash", user.passwordHash);
    query.bindValue(":role", user.role);
    query.bindValue(":is_active", user.isActive ? 1 : 0);

    if (!query.exec()) {
        qCritical() << "SQL Error (insertUser):" << query.lastError().text();
        return OperationResult::Fail("Користувач вже існує або помилка БД.");
    }
    return OperationResult::Ok();
}
