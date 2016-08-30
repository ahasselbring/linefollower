#pragma once

#include "framework.h"
#include "data/request.h"
#include "data/light.h"

MODULE(behavior_control)
  USES(light)
  PROVIDES(request)
  /// a timer for certain states
  unsigned int timer;
END_MODULE(behavior_control)
