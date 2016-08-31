#include "asuro.h"
#include "controller.h"

static void get_line_data(line_t* line)
{
  unsigned int sensor[2];
  LineData(sensor);
  line->left = sensor[LEFT];
  line->right = sensor[RIGHT];
}

static unsigned char convert_motor_mode(motor_mode_t mode)
{
  switch (mode) {
    case MOTOR_FORWARD:
      return FWD;
    case MOTOR_BACKWARD:
      return RWD;
    case MOTOR_BREAK:
      return BREAK;
    case MOTOR_OFF:
      return FREE;
  }
  return FREE;
}

static void set_motor_data(const control_t* control)
{
  MotorDir(convert_motor_mode(control->left_mode), convert_motor_mode(control->right_mode));
  MotorSpeed(control->left, control->right);
}

int main()
{
  controller_t controller;
  Init();
  FrontLED(ON);
  controller_init(&controller, &get_line_data, &set_motor_data);
  while (1) {
    controller_execute(&controller);
    // TODO: do something about timing
  }
}
