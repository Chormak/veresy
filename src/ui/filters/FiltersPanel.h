/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef FILTERSPANEL_H
#define FILTERSPANEL_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>

class FiltersPanel : public QWidget {
  Q_OBJECT
public:
  explicit FiltersPanel(QWidget *parent = nullptr);

  QString filterText() const { return m_searchEdit->text(); }
  int filterMode() const {return m_statusFilterCombo->currentIndex(); }

  void focusSearch();

signals:
  void filtersChanged();

private:
  void setupUi();

  QLineEdit *m_searchEdit;
  QComboBox *m_statusFilterCombo;
};

#endif
