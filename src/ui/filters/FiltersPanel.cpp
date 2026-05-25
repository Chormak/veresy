/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "FiltersPanel.h"

FiltersPanel::FiltersPanel(QWidget *parent) : QWidget(parent) {
  setupUi();
}

void FiltersPanel::setupUi() {
  auto *topLayout = new QHBoxLayout(this);
  topLayout->setContentsMargins(0, 0, 0, 0);

  m_searchEdit = new QLineEdit(this);
  m_searchEdit->setPlaceholderText("Пошук за клієнтом або пристроєм...");
  topLayout->addWidget(m_searchEdit, 3);

  m_statusFilterCombo = new QComboBox(this);
  m_statusFilterCombo->addItems({
    "Всі замовлення",
    "Тільки активні",
    "Тільки завершені"
  });
  topLayout->addWidget(m_statusFilterCombo, 1);

  connect(m_searchEdit, &QLineEdit::textChanged, this, &FiltersPanel::filtersChanged);
  connect(m_statusFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FiltersPanel::filtersChanged);
}

void FiltersPanel::focusSearch() {
  m_searchEdit->setFocus();
  m_searchEdit->selectAll();
}
