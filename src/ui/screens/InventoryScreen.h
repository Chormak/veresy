/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef INVENTORYSCREEN_H
#define INVENTORYSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class InventoryScreen : public QWidget {
  Q_OBJECT
public:
  explicit InventoryScreen(QWidget *parent = nullptr) : QWidget(parent) {
    auto *layout = new QVBoxLayout(this);
    auto *title = new QLabel("Модуль: СКЛАД ТА ЗАПЧАСТИНИ\n(Буде реалізовано...)", this);
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
  }
};

#endif
