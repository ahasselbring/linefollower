#ifdef TARGET_ROBOT
#include "asuro.h"
#elif TARGET_SIMULATOR
#include <unistd.h>
#include "simulator.h"

#define SIMULATION_DT 0.01
#define SIMULATION_USLEEP 50000
#else
#error You need to define a target
#endif

#include "data/line.h"
#include "data/request.h"
#include "data/light.h"
#include "data/state.h"
#include "data/control.h"

#include "module/behavior_control.h"
#include "module/line_perception.h"
#include "module/modeling.h"
#include "module/motor_control.h"

#define DECLARE_MODULE_GET(name) \
name##_t* global_get_##name() \
{ \
  static name##_t name; \
  return &name; \
}

DECLARE_MODULE_GET(line)
DECLARE_MODULE_GET(request)
DECLARE_MODULE_GET(light)
DECLARE_MODULE_GET(state)
DECLARE_MODULE_GET(control)

#ifdef TARGET_SIMULATOR
static simulator_t simulator;
#endif

static void get_line_data()
{
  line_t* line = global_get_line();
#ifdef TARGET_ROBOT
  unsigned int sensor[2];
  LineData(sensor);
  line->left = sensor[LEFT];
  line->right = sensor[RIGHT];
#else
  simulator_get_line_data(&simulator, &(line->left), &(line->right));
#endif
}

static unsigned char convert_motor_mode(motor_mode_t mode)
{
  switch (mode) {
    case MOTOR_FORWARD:
#ifdef TARGET_ROBOT
      return FWD;
#else
      return 1;
#endif
    case MOTOR_BACKWARD:
#ifdef TARGET_ROBOT
      return RWD;
#else
      return 2;
#endif
    case MOTOR_BREAK:
#ifdef TARGET_ROBOT
      return BREAK;
#else
      return 3;
#endif
    case MOTOR_OFF:
#ifdef TARGET_ROBOT
      return FREE;
#else
      return 4;
#endif
  }
#ifdef TARGET_ROBOT
  return FREE;
#else
  return 0;
#endif
}

static void set_motor_data()
{
  const control_t* control = global_get_control();
#ifdef TARGET_ROBOT
  MotorDir(convert_motor_mode(control->left_mode), convert_motor_mode(control->right_mode));
  MotorSpeed(control->left, control->right);
#else
  simulator_set_motor_data(&simulator, control->left, control->right, convert_motor_mode(control->left_mode), convert_motor_mode(control->right_mode));
#endif
}

int main()
{
  // declare modules
  behavior_control_t behavior_control;
  line_perception_t line_perception;
  modeling_t modeling;
  motor_control_t motor_control;
  // call module initializers
  motor_control_init(&motor_control);
  modeling_init(&modeling);
  behavior_control_init(&behavior_control);
  line_perception_init(&line_perception);
#ifdef TARGET_ROBOT
  Init();
  // the front LED only exists on the real robot
  FrontLED(ON);
#else
  simulator_init(&simulator);
#endif
  while (1) {
    get_line_data();
    DEBUG_OUTPUT("\n-+~+- New cycle -+~+-\n");
    behavior_control_execute(&behavior_control);
    line_perception_execute(&line_perception);
    modeling_execute(&modeling);
    motor_control_execute(&motor_control);
    set_motor_data();
#ifdef TARGET_SIMULATOR
    simulator_cycle(&simulator, SIMULATION_DT);
    usleep(SIMULATION_USLEEP);
#endif
  }
}
