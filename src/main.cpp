/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "app/Application.h"
#include "utils/Logger.h"
#include "core/users/UserManager.h"
#include "ui/auth/LoginDialog.h"
#include "ui/MainWindow.h"

int main (int argc, char *argv[]) {
  Logger::init();

  qInfo() <<"==================================================";
  qInfo() <<"APPLICATION START: Запуск системи Veresy...";
  qInfo() <<"Режим збірки: v0.10.1-alpha";
  Application veresyApp(argc, argv);
  auto userManager = std::make_shared<UserManager>();

  LoginDialog loginDialog(userManager.get());
  if (loginDialog.exec() != QDialog::Accepted) {
      qInfo() << "STARTUP: Вхід скасовано користувачем. Завершення.";
      qInfo() <<"=================================================";
      return 0;
  }
  qInfo() <<"STARTUP: Завантаження графічних модулів інтерфейсу...";
  MainWindow win;
  win.show();
  qInfo() <<"=================================================";

  return veresyApp.exec();

}
