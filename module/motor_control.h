#pragma once

#include "framework.h"
#include "data/request.h"
#include "data/state.h"
#include "data/control.h"

MODULE(motor_control)
  REQUIRES(request)
  REQUIRES(state)
  PROVIDES(control)
  /// the integral over the error
  signed int integral;
  /// the last measured error (used for derivative)
  signed int last_error;
END_MODULE(motor_control)
