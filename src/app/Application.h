/*
* Copyright 2026 Maksym Chornyi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>
#include <QMainWindow>
#include <memory>
#include "../ui/MainWindow.h"
#include "../data/Database.h"

class Application {
  public:
    Application(int &argc, char **argv);
    ~Application();
    int exec();
  private:
    std::unique_ptr<QApplication> m_app;

    std::unique_ptr<Database> m_db;

    std::unique_ptr<MainWindow> m_mainWindow;
};

#endif
