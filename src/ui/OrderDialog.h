/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef ORDERDIALOG_H
#define ORDERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QKeyEvent>
#include "../core/orders/Order.h"

class OrderDialog : public QDialog {
  Q_OBJECT

public:
  explicit OrderDialog(QWidget *parent = nullptr);

  void setOrderData(const Order& order);
  void focusFirstField();

  QString getClientName() const { return m_clientEdit->text(); }
  QString getDevice() const { return m_deviceEdit->text(); }
  QString getIssue() const { return m_issueEdit->text(); }

  void reject() override;

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
  void setDirty();

private:
  QLineEdit *m_clientEdit;
  QLineEdit *m_deviceEdit;
  QLineEdit *m_issueEdit;
  QPushButton *m_btnSave;
  int m_currentOrderId = 0;
  OrderStatus m_currentStatus = OrderStatus::Created;
  QDateTime m_currentCreatedAt;

  bool m_isDirty = false;
  bool m_isInitializing = false;
};

#endif
