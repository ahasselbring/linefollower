#include "motor_control.h"

// FIXME: needs to be adapted for the real robot
#define Kl -0.5
// FIXME: needs to be adapted for the real robot
#define Kr 0.5

MODULE_INIT(motor_control)
{
  MODULE_GET(request);
  MODULE_GET(state);
  MODULE_GET(control);
  self->control->left = 0;
  self->control->right = 0;
  self->control->left_mode = MOTOR_BREAK;
  self->control->right_mode = MOTOR_BREAK;
}

static void controller(signed int reference, signed int state, unsigned char* left, unsigned char* right)
{
  signed int error = reference - state;
  DEBUG_OUTPUT("MC: e = %d\n", error);
  // P controller
  if (Kl * error > 75) {
    *left = 255;
  } else if (Kl * error < -180) {
    *left = 0;
  } else {
    *left = 180 + Kl * error;
  }
  if (Kr * error > 75) {
    *right = 255;
  } else if (Kr * error < -180) {
    *right = 0;
  } else {
    *right = 180 + Kr * error;
  }
  DEBUG_OUTPUT("MC: u = [ %d %d ]\n", *left, *right);
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
      controller(0, self->state->x, &(self->control->left), &(self->control->right));
      self->control->left_mode = MOTOR_FORWARD;
      self->control->right_mode = MOTOR_FORWARD;
      break;
    case REQUEST_CHECK_FINISHED:
      if (self->state->x < 0) {
        controller(-100, self->state->x, &(self->control->left), &(self->control->right));
      } else {
        controller(100, self->state->x, &(self->control->left), &(self->control->right));
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
