#include <QApplication>

#include "MainWindow.hpp"
#include "SimulatorCycleBundle.hpp"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  qRegisterMetaType<SimulatorCycleBundle>();
  MainWindow window;
  window.show();
  return app.exec();
}
