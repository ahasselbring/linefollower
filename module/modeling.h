#pragma once

#include "framework.h"
#include "data/request.h"
#include "data/light.h"
#include "data/control.h"
#include "data/state.h"

MODULE(modeling)
  REQUIRES(request)
  REQUIRES(light)
  USES(control)
  PROVIDES(state)
END_MODULE(modeling)
