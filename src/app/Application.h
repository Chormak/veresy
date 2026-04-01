#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>
#include <QMainWindow>
#include <memory>
#include "../ui/MainWindow.h"

class Application {
  public:
    Application(int &argc, char **argv);
    ~Application();
    int exec();
  private:
    std::unique_ptr<QApplication> m_app;
    std::unique_ptr<MainWindow> m_mainWindow;
};

#endif
