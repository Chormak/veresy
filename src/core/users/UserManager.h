/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <memory>
#include "User.h"
#include "../orders/Order.h"
#include "../../data/repositories/UserRepository.h"

class Q_DECL_EXPORT UserManager : public QObject {
    Q_OBJECT
public:
    explicit Q_DECL_EXPORT UserManager(QObject *parent = nullptr);

    OperationResult login(const QString& username, const QString& password);
    void logout();

    User currentUser() const { return m_currentUser; }
    bool isLoggedIn() const { return m_currentUser.id > 0; }

private:
    User m_currentUser;
    std::unique_ptr<UserRepository> m_repository;
};

#endif
