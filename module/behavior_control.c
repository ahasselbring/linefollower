#include "behavior_control.h"

#define DRIVE_TIMEOUT 100
#define CHECK_FINISHED_TIMEOUT 100

MODULE_INIT(behavior_control)
{
  MODULE_GET(light);
  MODULE_GET(request);
  this->request->type = REQUEST_NONE;
  this->timer = 0;
}

MODULE_EXECUTE(behavior_control)
{
  switch (this->request->type) {
    case REQUEST_NONE:
      DEBUG_OUTPUT("BC: In state NONE.\n");
      this->request->type = REQUEST_INIT;
      break;
    case REQUEST_INIT:
      DEBUG_OUTPUT("BC: In state INIT.\n");
      if (this->light->calibrated) {
        this->request->type = REQUEST_DRIVE;
        this->timer = 0;
      }
      break;
    case REQUEST_DRIVE:
      DEBUG_OUTPUT("BC: In state DRIVE.\n");
      if (this->light->dark) {
        this->timer = 0;
      } else if (this->timer > DRIVE_TIMEOUT) {
        this->request->type = REQUEST_CHECK_FINISHED;
        this->timer = 0;
      } else {
        this->timer++;
      }
      break;
    case REQUEST_CHECK_FINISHED:
      DEBUG_OUTPUT("BC: In state CHECK_FINISHED.\n");
      if (this->light->dark) {
        this->request->type = REQUEST_DRIVE;
        this->timer = 0;
      } else if (this->timer > CHECK_FINISHED_TIMEOUT) {
        this->request->type = REQUEST_FINISHED;
      } else {
        this->timer++;
      }
    case REQUEST_FINISHED:
      DEBUG_OUTPUT("BC: In state DRIVE.\n");
      break;
  }
}
