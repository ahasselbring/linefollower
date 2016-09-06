#include "behavior_control.h"

// FIXME: depends on cycle time
#define DRIVE_TIMEOUT 100
// FIXME: depends on cycle time
#define CHECK_FINISHED_TIMEOUT 100

MODULE_INIT(behavior_control)
{
  MODULE_GET(light);
  MODULE_GET(request);
  self->request->type = REQUEST_NONE;
  self->timer = 0;
}

MODULE_EXECUTE(behavior_control)
{
  switch (self->request->type) {
    case REQUEST_NONE:
      DEBUG_PRINT("BC: In state NONE.\n");
      self->request->type = REQUEST_INIT;
      break;
    case REQUEST_INIT:
      DEBUG_PRINT("BC: In state INIT.\n");
      if (self->light->calibrated) {
        self->request->type = REQUEST_DRIVE;
        self->timer = 0;
      }
      break;
    case REQUEST_DRIVE:
      DEBUG_PRINT("BC: In state DRIVE.\n");
      if (self->light->dark) {
        self->timer = 0;
      } else if (self->timer > DRIVE_TIMEOUT) {
        self->request->type = REQUEST_CHECK_FINISHED;
        self->timer = 0;
      } else {
        self->timer++;
      }
      break;
    case REQUEST_CHECK_FINISHED:
      DEBUG_PRINT("BC: In state CHECK_FINISHED.\n");
      if (self->light->dark) {
        self->request->type = REQUEST_DRIVE;
        self->timer = 0;
      } else if (self->timer > CHECK_FINISHED_TIMEOUT) {
        self->request->type = REQUEST_FINISHED;
      } else {
        self->timer++;
      }
      break;
    case REQUEST_FINISHED:
      DEBUG_PRINT("BC: In state FINISHED.\n");
      break;
  }
}
