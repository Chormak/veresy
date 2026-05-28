/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "LoginDialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(UserManager* userManager, QWidget *parent)
    : QDialog(parent), m_userManager(userManager)
{
    setupUi();
}

void LoginDialog::setupUi() {
    setWindowTitle("Авторизація | Veresy");
    resize(300, 150);
    auto *layout = new QFormLayout(this);

    m_usernameEdit = new QLineEdit(this);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    layout->addRow("Користувач:", m_usernameEdit);
    layout->addRow("Пароль:", m_passwordEdit);

    m_btnLogin = new QPushButton("Увійти", this);
    layout->addWidget(m_btnLogin);

    connect(m_btnLogin, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);

    m_usernameEdit->setFocus();
}

void LoginDialog::onLoginClicked() {
    OperationResult result = m_userManager->login(m_usernameEdit->text(), m_passwordEdit->text());

    if (result.success) {
        accept();
    } else {
        QMessageBox::warning(this, "Помилка входу", result.errorMassage);
    }
}
