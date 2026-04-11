/*
 * Copyright 2026 Maksym Chornyi
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 */


#include "Application.h"
#include "../data/Database.h"

Application::Application(int &argc, char **argv) {
  m_app = std::make_unique<QApplication>(argc, argv);

  m_db = std::make_unique<Database>("veresy.db");
  if (m_db->open()) {
    m_db->createTable();
  }

  m_mainWindow = std::make_unique<MainWindow>();
  m_mainWindow->show();
}

Application::~Application() {
}

int Application::exec() {
  return m_app->exec();
}
