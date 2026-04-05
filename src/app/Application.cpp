#include "Application.h"
#include "../data/Database.h"

Application::Application(int &argc, char **argv) {
  m_app = std::make_unique<QApplication>(argc, argv);

  Database db("veresy.db");
  if (db.open()) {
    db.createTable();
  }

  m_mainWindow = std::make_unique<MainWindow>();
  m_mainWindow->show();
}

Application::~Application() {
}

int Application::exec() {
  return m_app->exec();
}
