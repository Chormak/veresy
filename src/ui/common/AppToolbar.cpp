/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/
 
#include "AppToolbar.h"

AppToolbar::AppToolbar(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void AppToolbar::setupUi() {
    auto *toolbarLayout = new QHBoxLayout(this);
    toolbarLayout->setContentsMargins(0, 0, 0, 0);

    m_filtersPanel = new FiltersPanel(this);
    toolbarLayout->addWidget(m_filtersPanel, 3);

    m_btnAddOrder = new QPushButton("Створити замовлення", this);
    toolbarLayout->addWidget(m_btnAddOrder, 1);

    connect(m_btnAddOrder, &QPushButton::clicked, this, &AppToolbar::addOrderRequested);

    connect(m_filtersPanel, &FiltersPanel::filtersChanged, this, &AppToolbar::filtersChanged);
}
