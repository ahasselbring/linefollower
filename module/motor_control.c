#include "motor_control.h"

#define Kl -0.5
#define Kr 0.5

MODULE_INIT(motor_control)
{
  MODULE_GET(request);
  MODULE_GET(state);
  MODULE_GET(control);
  this->control->left = 0;
  this->control->right = 0;
  this->control->left_mode = MOTOR_BREAK;
  this->control->right_mode = MOTOR_BREAK;
}

static void controller(signed int reference, signed int state, unsigned char* left, unsigned char* right)
{
  signed int error = reference - state;
  DEBUG_OUTPUT("MC: e = %d\n", error);
  // P controller
  *left = 180 + Kl * error;
  *right = 180 + Kr * error;
  DEBUG_OUTPUT("MC: u = [ %d %d ]\n", *left, *right);
}

MODULE_EXECUTE(motor_control)
{
  switch (this->request->type) {
    case REQUEST_NONE:
      this->control->left = 0;
      this->control->right = 0;
      this->control->left_mode = MOTOR_BREAK;
      this->control->right_mode = MOTOR_BREAK;
      break;
    case REQUEST_INIT:
      this->control->left = 255;
      this->control->right = 255;
      this->control->left_mode = MOTOR_FORWARD;
      this->control->right_mode = MOTOR_FORWARD;
      break;
    case REQUEST_DRIVE:
      controller(0, this->state->x, &(this->control->left), &(this->control->right));
      this->control->left_mode = MOTOR_FORWARD;
      this->control->right_mode = MOTOR_FORWARD;
      break;
    case REQUEST_CHECK_FINISHED:
      if (this->state->x < 0) {
        controller(-100, this->state->x, &(this->control->left), &(this->control->right));
      } else {
        controller(100, this->state->x, &(this->control->left), &(this->control->right));
      }
      this->control->left_mode = MOTOR_FORWARD;
      this->control->right_mode = MOTOR_FORWARD;
      break;
    case REQUEST_FINISHED:
      this->control->left = 0;
      this->control->right = 0;
      this->control->left_mode = MOTOR_BREAK;
      this->control->right_mode = MOTOR_BREAK;
      break;
  }
}
