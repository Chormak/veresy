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
