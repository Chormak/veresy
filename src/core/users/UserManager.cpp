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

    m_currentUser = user;
    qInfo() << "USER ACTION: Успішний вхід користувача" << m_currentUser.username << QString("[%1]").arg(m_currentUser.role);

    return OperationResult::Ok();
}

void UserManager::logout() {
    qInfo() << "USER ACTION: Вихід користувача з системи:" << m_currentUser.username;
    m_currentUser = User();
}
