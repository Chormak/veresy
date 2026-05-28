/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "Database.h"
#include <QCryptographicHash>

Database::Database(const QString& path) : m_path(path) {
  m_db = QSqlDatabase::addDatabase("QSQLITE");
  m_db.setDatabaseName(m_path);
}

Database::~Database() {
  close();
}

bool Database::open() {
  if (!m_db.open()) {
    qCritical() << "Помилка підключення до БД:" << m_db.lastError().text();
    return false;
  }
  qDebug() << "БД успішно відкрита за шляхом:" << m_path;
  return true;
}

void Database::close() {
  if (m_db.isOpen()) {
    m_db.close();
  }
}

bool Database::createTable() {
  QSqlQuery query;

  QString sqlOrders = "CREATE TABLE IF NOT EXISTS orders ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "client_name TEXT NOT NULL, "
                "device TEXT, "
                "issue TEXT, "
                "status INTEGER, "
                "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
                ");";
  if (!query.exec(sqlOrders)) {
    qCritical() << "Помилка створення таблиці:" << query.lastError().text();
    return false;
  }

  QString sqlHistory = "CREATE TABLE IF NOT EXISTS order_history ("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                       "order_id INTEGER NOT NULL, "
                       "old_status INTEGER, "
                       "new_status INTEGER NOT NULL, "
                       "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
                       "comment TEXT, "
                       "performed_by TEXT DEFAULT 'admin', "
                       "FOREIGN KEY(order_id) REFERENCES orders(id) ON DELETE CASCADE"
                       ");";

  if (!query.exec(sqlHistory)) {
    qCritical() << "SQL Error (create history table):" <<query.lastError().text();
    return false;
  }

  QString sqlUsers = "CREATE TABLE IF NOT EXISTS users ("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                       "username TEXT NOT NULL UNIQUE, "
                       "display_name TEXT, "
                       "password_hash TEXT NOT NULL, "
                       "role TEXT NOT NULL DEFAULT 'master', "
                       "created_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
                       "is_active INTEGER DEFAULT 1"
                       ");";

    if (!query.exec(sqlUsers)) {
        qCritical() << "SQL Error (create users table):" << query.lastError().text();
        return false;
    }

    query.exec("SELECT COUNT(*) FROM users");
    if (query.next() && query.value(0).toInt() == 0) {
        QString defaultPassword = "admin";
        QByteArray hash = QCryptographicHash::hash(defaultPassword.toUtf8(), QCryptographicHash::Sha256);
        QString passwordHash = QString(hash.toHex());

        QSqlQuery insertAdmin;
        insertAdmin.prepare("INSERT INTO users (username, display_name, password_hash, role) "
                            "VALUES ('admin', 'Головний Адміністратор', :hash, 'admin')");
        insertAdmin.bindValue(":hash", passwordHash);
        insertAdmin.exec();
        qInfo() << "DATABASE: Створено дефолтного користувача (admin / admin)";
    }
  qDebug() << "Таблиці бази даних готові до роботи!";
  return true;
}
