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

class OrderDialog : public QDialog {
  Q_OBJECT

public:
  OrderDialog(QWidget *parent = nullptr);

  QString getClientName() const { return m_clientEdit->text(); }
  QString getDevice() const { return m_deviceEdit->text(); }
  QString getIssue() const { return m_issueEdit->text(); }

private:
  QLineEdit *m_clientEdit;
  QLineEdit *m_deviceEdit;
  QLineEdit *m_issueEdit;
};

#endif
