/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "UsersView.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QComboBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QInputDialog>
#include <QCryptographicHash>

UsersView::UsersView(QWidget *parent) : QWidget(parent) {
    setupUi();
    reloadUsers();
}

void UsersView::setupUi() {
    auto *layout = new QVBoxLayout(this);

    m_btnCreateUser = new QPushButton("Додати користувача", this);
    layout->addWidget(m_btnCreateUser);

    m_usersTable = new QTableWidget(this);
    m_usersTable->setColumnCount(5);
    m_usersTable->setHorizontalHeaderLabels({"ID", "Логін", "Ім'я", "Роль", "Статус"});
    m_usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(m_usersTable);

    connect(m_btnCreateUser, &QPushButton::clicked, this, &UsersView::onCreateUserClicked);
}

void UsersView::reloadUsers() {
    m_usersTable->setRowCount(0);
    QSqlQuery query("SELECT id, username, display_name, role, is_active FROM users");

    int row = 0;
    while (query.next()) {
        m_usersTable->insertRow(row);
        int userId = query.value(0).toInt();
        QString username = query.value(1).toString();
        QString displayName = query.value(2).toString();
        QString role = query.value(3).toString();
        bool isActive = query.value(4).toInt() == 1;

        m_usersTable->setItem(row, 0, new QTableWidgetItem(QString::number(userId)));
        m_usersTable->setItem(row, 1, new QTableWidgetItem(username));
        m_usersTable->setItem(row, 2, new QTableWidgetItem(displayName));

        QComboBox *roleCombo = new QComboBox(this);
        roleCombo->addItems({"admin", "manager", "technician"});
        roleCombo->setCurrentText(role);
        roleCombo->setProperty("userId", userId);
        connect(roleCombo, &QComboBox::currentTextChanged, this, [this, roleCombo](const QString& text){
            int id = roleCombo->property("userId").toInt();
            QSqlQuery u; u.prepare("UPDATE users SET role = :r WHERE id = :id");
            u.bindValue(":r", text); u.bindValue(":id", id); u.exec();
        });
        m_usersTable->setCellWidget(row, 3, roleCombo);

        QPushButton *statusBtn = new QPushButton(isActive ? "Деактивувати" : "Активувати", this);
        statusBtn->setProperty("userId", userId);
        statusBtn->setProperty("status", isActive);
        connect(statusBtn, &QPushButton::clicked, this, [this, statusBtn](){
            int id = statusBtn->property("userId").toInt();
            bool current = statusBtn->property("status").toBool();
            QSqlQuery u; u.prepare("UPDATE users SET is_active = :a WHERE id = :id");
            u.bindValue(":a", current ? 0 : 1); u.bindValue(":id", id);
            if (u.exec()) reloadUsers();
        });
        m_usersTable->setCellWidget(row, 4, statusBtn);

        row++;
    }
}

void UsersView::onCreateUserClicked() {
    QString username = QInputDialog::getText(this, "Новий користувач", "Введіть логін:");
    if (username.trimmed().isEmpty()) return;
    QString password = QInputDialog::getText(this, "Новий користувач", "Введіть пароль:");
    if (password.isEmpty()) return;

    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    QString passwordHash = QString(hash.toHex()).toLower();

    QSqlQuery q;
    q.prepare("INSERT INTO users (username, display_name, password_hash, role) VALUES (:u, :d, :p, 'technician')");
    q.bindValue(":u", username.trimmed());
    q.bindValue(":d", username.trimmed());
    q.bindValue(":p", passwordHash);

    if (q.exec()) {
        QMessageBox::information(this, "Успіх", "Користувача створено зі стандартною роллю technician");
        reloadUsers();
    } else {
        QMessageBox::warning(this, "Помилка", "Не вдалося створити користувача (можливо логін зайнятий).");
    }
}

void UsersView::onRoleChanged(int row, const QString& newRole)
{
    Q_UNUSED(row);
    Q_UNUSED(newRole);
}

void UsersView::onStatusToggled(int row)
{
    Q_UNUSED(row);
}
