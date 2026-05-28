/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include "../../core/users/UserManager.h"

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(UserManager* userManager, QWidget *parent = nullptr);

private slots:
    void onLoginClicked();

private:
    void setupUi();

    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_btnLogin;
    UserManager *m_userManager;
};

#endif
