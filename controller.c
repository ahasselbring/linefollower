#include "controller.h"
#include "framework.h"

#include "data/line.h"
#include "data/request.h"
#include "data/light.h"
#include "data/state.h"
#include "data/control.h"

#define DECLARE_MODULE_GET(name) \
name##_t* global_get_##name() \
{ \
  static name##_t name; \
  return &name; \
}

// create getters for static instances of all data types
DECLARE_MODULE_GET(line)
DECLARE_MODULE_GET(request)
DECLARE_MODULE_GET(light)
DECLARE_MODULE_GET(state)
DECLARE_MODULE_GET(control)

void controller_execute(controller_t* self)
{
  DEBUG_OUTPUT("\n-+~+- Start of controller execution -+~+-\n");
  // get sensor data
  self->get_line_data(global_get_line());
  // execute all modules in the correct order
  behavior_control_execute(&(self->behavior_control));
  line_perception_execute(&(self->line_perception));
  modeling_execute(&(self->modeling));
  motor_control_execute(&(self->motor_control));
  // send commands to motors
  self->set_motor_data(global_get_control());
}

int controller_init(controller_t* self, void (*get_line_data)(), void (*set_motor_data)())
{
  // initialize hardware interface
  if (get_line_data == (void*)0 || set_motor_data == (void*)0) {
    DEBUG_OUTPUT("C: get_line_data or set_motor_data is NULL.\n");
    return -1;
  }
  self->get_line_data = get_line_data;
  self->set_motor_data = set_motor_data;
  // construct all modules
  behavior_control_init(&(self->behavior_control));
  line_perception_init(&(self->line_perception));
  modeling_init(&(self->modeling));
  motor_control_init(&(self->motor_control));
  return 0;
}
