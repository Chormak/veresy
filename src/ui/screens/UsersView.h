/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef USERSVIEW_H
#define USERSVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class UsersView : public QWidget {
    Q_OBJECT
public:
    explicit UsersView(QWidget *parent = nullptr);
    void reloadUsers();

private slots:
    void onCreateUserClicked();
    void onRoleChanged(int row, const QString& newRole);
    void onStatusToggled(int row);

private:
    void setupUi();
    QTableWidget *m_usersTable;
    QPushButton *m_btnCreateUser;
};

#endif
