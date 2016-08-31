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

void controller_execute(controller_t* this)
{
  // get sensor data
  this->get_line_data(global_get_line());
  // execute all modules in the correct order
  behavior_control_execute(&(this->behavior_control));
  line_perception_execute(&(this->line_perception));
  modeling_execute(&(this->modeling));
  motor_control_execute(&(this->motor_control));
  // send commands to motors
  this->set_motor_data(global_get_control());
}

void controller_init(controller_t* this, void (*get_line_data)(), void (*set_motor_data)())
{
  // initialize hardware interface
  if (get_line_data == NULL || set_motor_data == NULL) {
    DEBUG_OUTPUT("C: get_line_data or set_motor_data is NULL.\n");
    return;
  }
  this->get_line_data = get_line_data;
  this->set_motor_data = set_motor_data;
  // construct all modules
  behavior_control_init(&(this->behavior_control));
  line_perception_init(&(this->line_perception));
  modeling_init(&(this->modeling));
  motor_control_init(&(this->motor_control));
}
