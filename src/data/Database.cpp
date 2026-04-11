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

Database::Database(const QString& path) : m_path(path) {
  m_db = QSqlDatabase::addDatabase("QSQLITE");
  m_db.setDatabaseName(m_path);
}

Database::~Database() {
  close();
}

bool Database::open() {
  if (!m_db.open()) {
    qDebug() << "Помилка підключення до БД:" << m_db.lastError().text();
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

  QString sql = "CREATE TABLE IF NOT EXISTS orders ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "client_name TEXT NOT NULL, "
                "device TEXT, "
                "issue TEXT, "
                "status TEXT, "
                "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
                ");";
  if (!query.exec(sql)) {
    qDebug() << "Помилка створення таблиці:" << query.lastError().text();
    return false;
  }

  qDebug() << "Таблиця 'orders' готова!";
  return true;
}
