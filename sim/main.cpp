#include <QApplication>

#include "MainWindow.hpp"
#include "SimulatorCycleBundle.hpp"
#include "SimulatorLoadBundle.hpp"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  qRegisterMetaType<SimulatorCycleBundle>();
  qRegisterMetaType<SimulatorLoadBundle>();
  MainWindow window;
  window.show();
  return app.exec();
}
