/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QSqlQuery>

class Database {
  public:
    Database(const QString& path);
    ~Database();

    bool open();
    void close();
    bool createTable();

  private:
    QSqlDatabase m_db;
    QString m_path;
};

#endif
