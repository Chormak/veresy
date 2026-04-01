#include "Application.h"

Application::Application(int &argc, char **argv) {
  m_app = std::make_unique<QApplication>(argc, argv);
  m_mainWindow = std::make_unique<MainWindow>();
  m_mainWindow->show();
}

Application::~Application() {
}

int Application::exec() {
  return m_app->exec();
}
