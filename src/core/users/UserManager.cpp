/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "UserManager.h"
#include <QCryptographicHash>
#include "../auth/SessionManager.h"

UserManager::UserManager(QObject *parent) : QObject(parent) {
    m_repository = std::make_unique<UserRepository>();
}

OperationResult Q_DECL_EXPORT UserManager::login(const QString& username, const QString& password) {
    if (username.trimmed().isEmpty() || password.isEmpty()) {
        return OperationResult::Fail("Поля імені та паролю не можуть бути порожніми.");
    }

    User user = m_repository->getByUsername(username.trimmed());

    if (user.id == 0 || !user.isActive) {
        return OperationResult::Fail("Користувача не знайдено або деактивовано.");
    }

    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    QString inputHash = QString(hash.toHex());

    if (user.passwordHash != inputHash) {
        return OperationResult::Fail("Невірний пароль.");
    }

    SessionManager::instance().setSession(user);
    qInfo() << "USER ACTION [" << user.username << "]: Успішний вхід у систему. Роль:" << user.role;

    return OperationResult::Ok();
}

void UserManager::logout() {
    QString currentUsername = SessionManager::instance().currentUser().username;
    SessionManager::instance().clearSession();
    qInfo() << "USER ACTION [" << currentUsername << "]: Вихід із системи.";
}
