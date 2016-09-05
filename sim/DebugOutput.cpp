#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include "SimulationThread.hpp"

extern "C" void debug_output(const char* fmt, ...)
{
  char* str;
  va_list args;
  va_start(args, fmt);
  vasprintf(&str, fmt, args);
  va_end(args);
  SimulationThread::debug_output(QString(str));
  free(str);
}
