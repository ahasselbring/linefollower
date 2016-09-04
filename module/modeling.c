#include "modeling.h"

// FIXME: needs to be adapted to the real robot
#define LINE_SENSOR_DISTANCE 100
// FIXME: needs to be adapted to the real robot
#define BLACK_CENTER_DISTANCE 200
#define P0 (100 * 100)
// FIXME: depends on cycle time
#define B 0.1
// FIXME: depends on cycle time
#define Q 10 * 10
// FIXME: needs to be adapted to the real robot
#define Rll 5
// FIXME: needs to be adapted to the real robot
#define Rrr 5
// FIXME: needs to be adapted to the real robot
#define Rrl 0

MODULE_INIT(modeling)
{
  MODULE_GET(request);
  MODULE_GET(light);
  MODULE_GET(control);
  MODULE_GET(state);
  self->state->x = 0;
  self->state->p = P0;
}

static unsigned char expected_brightness(signed int x)
{
  if (x < -BLACK_CENTER_DISTANCE) {
    return 0;
  } else if (x < 0) {
    return 255 + x;
  } else if (x <= BLACK_CENTER_DISTANCE) {
    return 255 - x;
  } else {
    return 0;
  }
}

static signed int ddx_expected_brightness(signed int x)
{
  if (x < -BLACK_CENTER_DISTANCE) {
    return 0;
  } else if (x < 0) {
    return 1;
  } else if (x <= BLACK_CENTER_DISTANCE) {
    return -1;
  } else {
    return 0;
  }
}

MODULE_EXECUTE(modeling)
{
  if (self->request->type == REQUEST_INIT) {
    self->state->x = 0;
    self->state->p = P0;
    return;
  }
  // Kalman prediction
  self->state->x += B * (self->control->right - self->control->left);
  self->state->p += Q;
  DEBUG_OUTPUT("M: Predicted state: x = %d, p = %d\n", self->state->x, self->state->p);
  // Kalman update
  // y = [ y1 y2 ]' = [ self->light->left self->light->right ]' - h(self->state->x)
  const signed int y1 = (signed int)(self->light->left) - expected_brightness(self->state->x + LINE_SENSOR_DISTANCE);
  const signed int y2 = (signed int)(self->light->right) - expected_brightness(self->state->x - LINE_SENSOR_DISTANCE);
  DEBUG_OUTPUT("M: y = [ %d %d ]\n", y1, y2);
  // H = [ h1 h2 ]' = d/dx h(self->state->x)
  const signed int h1 = ddx_expected_brightness(self->state->x + LINE_SENSOR_DISTANCE);
  const signed int h2 = ddx_expected_brightness(self->state->x - LINE_SENSOR_DISTANCE);
  DEBUG_OUTPUT("M: h = [ %d %d ]\n", h1, h2);
  // S = [ s11 s12; s21 s22 ] = [ h1 h2 ]' * self->state->p * [ h1 h2 ] + [ r11 r12; r12 r22 ] = [ p * h1^2 + r11, p * h1 * h2 + r12; p * h1 * h2 + r12, p * h2^2 + r22 ]
  const signed int detS = (signed int)(self->state->p) * (Rll * h2 * h2 + Rrr * h1 * h1 - 2 * h1 * h2 * Rrl) + Rll * Rrr - Rrl * Rrl;
  if (detS == 0) {
    DEBUG_OUTPUT("M: S is not invertible!\n");
    return;
  }
  DEBUG_OUTPUT("M: det(S) = %d\n", detS);
  const signed int is11detS = ((signed int)(self->state->p) * h2 * h2 + Rrr);
  const signed int is12detS = -((signed int)(self->state->p) * h1 * h2 + Rrl);
  const signed int is22detS = ((signed int)(self->state->p) * h1 * h1 + Rll);
  DEBUG_OUTPUT("M: inv(S) * det(S) = [ %d %d; %d %d ]\n", is11detS, is12detS, is12detS, is22detS);
  // K = [ k1 k2 ] = self->state->p * [ h1 h2 ] * inv(S)
  const signed int k1detS = (signed int)(self->state->p) * (h1 * is11detS + h2 * is12detS);
  const signed int k2detS = (signed int)(self->state->p) * (h1 * is12detS + h2 * is22detS);
  DEBUG_OUTPUT("M: k * det(S) = [ %d %d ]\n", k1detS, k2detS);
  // x = x + K * y = x + [ k1 k2 ] * [ y1 y2 ]' = x + k1 * y1 + k2 * y2
  self->state->x += (k1detS * y1 + k2detS * y2) / detS;
  // P = (I - K * H) * P = (1 - [ k1 k2 ] * [ h1 h2 ]') * P
  self->state->p -= (k1detS * h1 + k2detS * h2) * (signed int)(self->state->p) / detS;
  DEBUG_OUTPUT("M: Estimated state: x = %d, p = %d\n", self->state->x, self->state->p);
}
