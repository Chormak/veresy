/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include <QMessageBox>
#include "OrderDialog.h"

OrderDialog::OrderDialog(QWidget *parent) : QDialog(parent) {
  setWindowTitle("Нове замовлення");
  auto *layout = new QFormLayout(this);

  m_clientEdit = new QLineEdit(this);
  m_deviceEdit = new QLineEdit(this);
  m_issueEdit = new QLineEdit(this);

  layout->addRow("Клієнт:", m_clientEdit);
  layout->addRow("Пристрій:", m_deviceEdit);
  layout->addRow("Проблема:", m_issueEdit);

  m_btnSave = new QPushButton("Зберегти" , this);
  auto *btnCancel = new QPushButton("Скасувати", this);
  layout->addWidget(m_btnSave);
  layout->addWidget(btnCancel);

  setTabOrder(m_clientEdit, m_deviceEdit);
  setTabOrder(m_deviceEdit, m_issueEdit);
  setTabOrder(m_issueEdit, m_btnSave);
  setTabOrder(m_btnSave, btnCancel);

  m_clientEdit->installEventFilter(this);
  m_deviceEdit->installEventFilter(this);
  m_issueEdit->installEventFilter(this);

  connect(m_btnSave, &QPushButton::clicked, this, &QDialog::accept);
  connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);

  connect(m_clientEdit, &QLineEdit::textChanged, this, &OrderDialog::setDirty);
  connect(m_deviceEdit, &QLineEdit::textChanged, this, &OrderDialog::setDirty);
  connect(m_issueEdit, &QLineEdit::textChanged, this, &OrderDialog::setDirty);

  focusFirstField();
}

void OrderDialog::focusFirstField() {
  m_clientEdit->setFocus();
  m_clientEdit->selectAll();
}

bool OrderDialog::eventFilter(QObject *watched, QEvent *event) {
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
      if (watched == m_clientEdit) {
        m_deviceEdit->setFocus();
        return true;
      }
      else if (watched == m_deviceEdit) {
        m_issueEdit->setFocus();
        return true;
      }
      else if (watched == m_issueEdit) {
        m_btnSave->setFocus();
        return true;
      }
    }
  }
  return QDialog::eventFilter(watched, event);
}

void OrderDialog::setDirty() {
  if (m_isInitializing) return;
  m_isDirty = true;
}

void OrderDialog::setOrderData(const Order& order) {
  m_isInitializing = true;

  setWindowTitle("Редагування замдвлення №" + QString::number(order.id));

  m_currentOrderId = order.id;
  m_currentStatus = order.status;
  m_currentCreatedAt = order.createdAt;

  m_clientEdit->setText(order.clientName);
  m_deviceEdit->setText(order.device);
  m_issueEdit->setText(order.issue);

  m_isInitializing = false;
  m_isDirty = false;
}

void OrderDialog::reject() {
  if (m_isDirty) {
    QMessageBox::StandardButton res = QMessageBox::warning(this, "Попередження",
                                                           "У вас є незбережені зміни! Ви дійсно хочете закрити вікно?",
                                                           QMessageBox::Yes | QMessageBox::No);
    if (res == QMessageBox::No) {
      return;
    }
  }
  QDialog::reject();
}
