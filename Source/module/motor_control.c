#include "motor_control.h"

// FIXME: needs to be adapted for the real robot
#define Kl -0.2
// FIXME: needs to be adapted for the real robot
#define Kr 0.2
// FIXME: needs to be adapted for the real robot
#define Ti 2
// FIXME: needs to be adapted for the real robot
#define Td 16
// FIXME: needs to be adapted for the real robot
#define CObias 180

MODULE_INIT(motor_control)
{
  MODULE_GET(request);
  MODULE_GET(state);
  MODULE_GET(control);
  self->control->left = 0;
  self->control->right = 0;
  self->control->left_mode = MOTOR_BREAK;
  self->control->right_mode = MOTOR_BREAK;
  self->integral = 0;
  self->last_error = 0;
}

static void controller(motor_control_t* self, signed int reference, signed int state)
{
  signed int error = reference - state;
  DEBUG_PRINT("MC: e = %d\n", error);
  self->integral += error;
  DEBUG_PRINT("MC: integral(e) = %d\n", self->integral);
  signed int derivative = error - self->last_error;
  DEBUG_PRINT("MC: derivative(e) = %d\n", derivative);
  self->last_error = error;
  // PID controller
  signed int left_ctrl = Kl * error + Kl * self->integral / Ti + Kl * derivative * Td;
  if (left_ctrl > 75) {
    self->control->left = 255;
  } else if (left_ctrl < -CObias) {
    self->control->left = 0;
  } else {
    self->control->left = CObias + left_ctrl;
  }
  signed int right_ctrl = Kr * error + Kr * self->integral / Ti + Kr * derivative * Td;
  if (right_ctrl > 75) {
    self->control->right = 255;
  } else if (right_ctrl < -CObias) {
    self->control->right = 0;
  } else {
    self->control->right = CObias + right_ctrl;
  }
  DEBUG_PRINT("MC: u = [ %d %d ]\n", self->control->left, self->control->right);
}

MODULE_EXECUTE(motor_control)
{
  switch (self->request->type) {
    case REQUEST_NONE:
      self->control->left = 0;
      self->control->right = 0;
      self->control->left_mode = MOTOR_BREAK;
      self->control->right_mode = MOTOR_BREAK;
      break;
    case REQUEST_INIT:
      self->control->left = 255;
      self->control->right = 255;
      self->control->left_mode = MOTOR_FORWARD;
      self->control->right_mode = MOTOR_FORWARD;
      break;
    case REQUEST_DRIVE:
      controller(self, 0, self->state->x);
      self->control->left_mode = MOTOR_FORWARD;
      self->control->right_mode = MOTOR_FORWARD;
      break;
    case REQUEST_CHECK_FINISHED:
      if (self->state->x < 0) {
        controller(self, -100, self->state->x);
      } else {
        controller(self, 100, self->state->x);
      }
      self->control->left_mode = MOTOR_FORWARD;
      self->control->right_mode = MOTOR_FORWARD;
      break;
    case REQUEST_FINISHED:
      self->control->left = 0;
      self->control->right = 0;
      self->control->left_mode = MOTOR_BREAK;
      self->control->right_mode = MOTOR_BREAK;
      break;
  }
}
