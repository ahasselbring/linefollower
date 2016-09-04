#include "line_perception.h"

// FIXME: needs to be adapted for the real robot
#define DARK_THRESHOLD 100
// FIXME: needs to be adapted for the real robot
#define MIN_WHITE_LINE 950
// FIXME: needs to be adapted for the real robot
#define MAX_BLACK_LINE 850
// FIXME: depends on cycle time
#define CALIBRATION_TIMEOUT 20

MODULE_INIT(line_perception)
{
  MODULE_GET(line);
  MODULE_GET(request);
  MODULE_GET(light);
  self->light->left = 255;
  self->light->right = 255;
  self->light->calibrated = 0;
  self->light->dark = 1;
  self->left_black_level = MAX_BLACK_LINE;
  self->left_white_level = MIN_WHITE_LINE;
  self->right_black_level = MAX_BLACK_LINE;
  self->right_white_level = MIN_WHITE_LINE;
  self->calibration_timer = 0;
  self->calibration_mask = 0;
}

MODULE_EXECUTE(line_perception)
{
  if (self->request->type == REQUEST_INIT) {
    if (self->line->left < self->left_black_level) {
      DEBUG_OUTPUT("LP: Reset timer for left black level (%u).\n", self->line->left);
      self->left_black_level = self->line->left;
      self->calibration_timer = 0;
      self->calibration_mask |= 0x01;
    }
    if (self->line->right < self->right_black_level) {
      DEBUG_OUTPUT("LP: Reset timer for right black level (%u).\n", self->line->right);
      self->right_black_level = self->line->right;
      self->calibration_timer = 0;
      self->calibration_mask |= 0x02;
    }
    if (self->line->left > self->left_white_level) {
      DEBUG_OUTPUT("LP: Reset timer for left white level (%u).\n", self->line->left);
      self->left_white_level = self->line->left;
      self->calibration_timer = 0;
      self->calibration_mask |= 0x04;
    }
    if (self->line->right > self->right_white_level) {
      DEBUG_OUTPUT("LP: Reset timer for right white level (%u).\n", self->line->right);
      self->right_white_level = self->line->right;
      self->calibration_timer = 0;
      self->calibration_mask |= 0x08;
    }
    self->calibration_timer++;
    if (self->calibration_mask == 0x0f && self->calibration_timer > CALIBRATION_TIMEOUT) {
      DEBUG_OUTPUT("LP: Done with calibration.\n");
      self->light->calibrated = 1;
    } else {
      self->light->calibrated = 0;
    }
    self->light->dark = 1;
  } else {
    if (self->line->left <= self->left_black_level) {
      self->light->left = 0;
    } else if (self->line->left >= self->left_white_level) {
      self->light->left = 255;
    } else {
      self->light->left = 255 * (self->line->left - self->left_black_level) / (self->left_white_level - self->left_black_level);
    }
    if (self->line->right <= self->right_black_level) {
      self->light->right = 0;
    } else if (self->line->right >= self->right_white_level) {
      self->light->right = 255;
    } else {
      self->light->right = 255 * (self->line->right - self->right_black_level) / (self->right_white_level - self->right_black_level);
    }
    DEBUG_OUTPUT("LP: Measurement: [ %d %d ]\n", self->light->left, self->light->right);
    self->light->calibrated = 1;
    self->light->dark = (self->light->left < DARK_THRESHOLD) || (self->light->right < DARK_THRESHOLD);
    DEBUG_OUTPUT("LP: Dark? %s.\n", (self->light->dark ? "yes" : "no"));
    self->calibration_timer = 0;
    self->calibration_mask = 0;
  }
}
