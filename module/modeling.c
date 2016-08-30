#include "modeling.h"

#define LINE_SENSOR_DISTANCE 10000
#define P0 (100 * 100)
#define Bl 1
#define Br 1
#define Q 10 * 10
#define Rll 1
#define Rrr 1
#define Rrl 1

MODULE_INIT(modeling)
{
  MODULE_GET(request);
  MODULE_GET(light);
  MODULE_GET(control);
  MODULE_GET(state);
  this->state->x = 0;
  this->state->p = P0;
}

static unsigned char expected_brightness(signed int x)
{
  if (x > -100 && x < 100) {
    return 255;
  }
  // TODO
  return 0;
}

static signed int ddx_expected_brightness(signed int x)
{
  // TODO
  return x;
}

MODULE_EXECUTE(modeling)
{
  if (this->request->type == REQUEST_INIT) {
    this->state->x = 0;
    this->state->p = P0;
    return;
  }
  // Kalman prediction
  this->state->x += Bl * this->control->left + Br * this->control->right;
  this->state->p += Q;
  // Kalman update
  // y = [ y1 y2 ]' = [ this->light->left this->light->right ]' - h(this->state->x)
  signed int y1 = (signed int)(this->light->left) - expected_brightness(this->state->x + LINE_SENSOR_DISTANCE);
  signed int y2 = (signed int)(this->light->right) - expected_brightness(this->state->x - LINE_SENSOR_DISTANCE);
  DEBUG_OUTPUT("M: y = [ %d %d ]\n", y1, y2);
  // H = [ h1 h2 ]' = d/dx h(this->state->x)
  signed int h1 = ddx_expected_brightness(this->state->x + LINE_SENSOR_DISTANCE);
  signed int h2 = ddx_expected_brightness(this->state->x - LINE_SENSOR_DISTANCE);
  DEBUG_OUTPUT("M: h = [ %d %d ]\n", h1, h2);
  // S = [ s11 s12; s21 s22 ] = [ h1 h2 ]' * this->state->p * [ h1 h2 ] + [ r11 r12; r12 r22 ] = [ p * h1^2 + r11, p * h1 * h2 + r12; p * h1 * h2 + r12, p * h2^2 + r22 ]
  signed detS = this->state->p * (Rll * h2 * h2 + Rrr * h1 * h1 - 2 * h1 * h2 * Rrl) + Rll * Rrr - Rrl * Rrl;
  if (detS == 0) {
    // TODO
    return;
  }
  signed int is11 = (this->state->p * h2 * h2 + Rrr) / detS;
  signed int is12 = -(this->state->p * h1 * h2 + Rrl) / detS;
  signed int is22 = (this->state->p * h1 * h1 + Rll) / detS;
  // K = [ k1 k2 ] = this->state->p * [ h1 h2 ] * inv(S)
  signed int k1 = this->state->p * (h1 * is11 + h2 * is12);
  signed int k2 = this->state->p * (h1 * is12 + h2 * is22);
  // x = x + K * y = x + [ k1 k2 ] * [ y1 y2 ]' = x + k1 * y1 + k2 * y2
  this->state->x += k1 * y1 + k2 * y2;
  // P = (I - K * H) * P = (1 - [ k1 k2 ] * [ h1 h2 ]') * P
  this->state->p -= (k1 * h1 + k2 * h2) * this->state->p;
  DEBUG_OUTPUT("M: Estimated state: x = %d, p = %d\n", this->state->x, this->state->p);
}
