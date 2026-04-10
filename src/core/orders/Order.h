#ifndef ORDER_H
#define ORDER_H
#include <QString>
#include <QDateTime>

struct Order {
  int id;
  QString clientName;
  QString device;
  QString issue;
  QString status;
  QDateTime createdAt;
};

#endif
