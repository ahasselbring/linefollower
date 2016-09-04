#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include <QApplication>

#include "MainWindow.hpp"

extern "C" void debug_output(const char* fmt, ...)
{
  char* str;
  va_list args;
  va_start(args, fmt);
  vasprintf(&str, fmt, args);
  va_end(args);
  MainWindow* main_window;
  foreach (QWidget* widget, QApplication::topLevelWidgets()) {
    main_window = qobject_cast<MainWindow*>(widget);
    if (main_window != nullptr) {
      main_window->add_to_debug(QString(str));
    }
  }
  free(str);
}
