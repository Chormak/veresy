/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef APPTOOLBAR_H
#define APPTOOLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include "../filters/FiltersPanel.h"

class AppToolbar : public QWidget {
    Q_OBJECT
public:
    explicit AppToolbar(QWidget *parent = nullptr);

    QString filterText() const { return m_filtersPanel->filterText(); }
    int filterMode() const { return m_filtersPanel->filterMode(); }

    void focusSearch() { m_filtersPanel->focusSearch(); }

signals:
    void addOrderRequested();
    void filtersChanged();

private:
    void setupUi();

    FiltersPanel *m_filtersPanel;
    QPushButton *m_btnAddOrder;
};

#endif
