#pragma once

#include "framework.h"
#include "data/request.h"
#include "data/state.h"
#include "data/control.h"

MODULE(motor_control)
  REQUIRES(request)
  REQUIRES(state)
  PROVIDES(control)
END_MODULE(motor_control)
