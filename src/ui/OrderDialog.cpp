/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

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

  auto *btnSave = new QPushButton("Зберегти" , this);
  layout->addWidget(btnSave);

  connect(btnSave, &QPushButton::clicked, this, &QDialog::accept);
}
