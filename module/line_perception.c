#include "line_perception.h"

#define DARK_THRESHOLD 200
#define MIN_WHITE_LINE 950
#define MAX_BLACK_LINE 850
#define CALIBRATION_TIMEOUT 20

MODULE_INIT(line_perception)
{
  MODULE_GET(line);
  MODULE_GET(request);
  MODULE_GET(light);
  this->light->left = 255;
  this->light->right = 255;
  this->light->calibrated = 0;
  this->light->dark = 1;
  this->left_black_level = MAX_BLACK_LINE;
  this->left_white_level = MIN_WHITE_LINE;
  this->right_black_level = MAX_BLACK_LINE;
  this->right_white_level = MIN_WHITE_LINE;
  this->calibration_timer = 0;
  this->calibration_mask = 0;
}

MODULE_EXECUTE(line_perception)
{
  if (this->request->type == REQUEST_INIT) {
    if (this->line->left < this->left_black_level) {
      DEBUG_OUTPUT("Reset timer for left black level (%u).\n", this->line->left);
      this->left_black_level = this->line->left;
      this->calibration_timer = 0;
      this->calibration_mask |= 0x01;
    }
    if (this->line->right < this->right_black_level) {
      DEBUG_OUTPUT("Reset timer for right black level (%u).\n", this->line->right);
      this->right_black_level = this->line->right;
      this->calibration_timer = 0;
      this->calibration_mask |= 0x02;
    }
    if (this->line->left > this->left_white_level) {
      DEBUG_OUTPUT("Reset timer for left white level (%u).\n", this->line->left);
      this->left_white_level = this->line->left;
      this->calibration_timer = 0;
      this->calibration_mask |= 0x04;
    }
    if (this->line->right > this->right_white_level) {
      DEBUG_OUTPUT("Reset timer for right white level (%u).\n", this->line->right);
      this->right_white_level = this->line->right;
      this->calibration_timer = 0;
      this->calibration_mask |= 0x08;
    }
    this->calibration_timer++;
    if (this->calibration_mask == 0x0f && this->calibration_timer > CALIBRATION_TIMEOUT) {
      this->light->calibrated = 1;
    } else {
      this->light->calibrated = 0;
    }
    this->light->dark = 1;
  } else {
    if (this->line->left <= this->left_black_level) {
      this->light->left = 0;
    } else if (this->line->left >= this->left_white_level) {
      this->light->left = 255;
    } else {
      this->light->left = 255 * (this->line->left - this->left_black_level) / (this->left_white_level - this->left_black_level);
    }
    if (this->line->right <= this->right_black_level) {
      this->light->right = 0;
    } else if (this->line->right >= this->right_white_level) {
      this->light->right = 255;
    } else {
      this->light->right = 255 * (this->line->right - this->right_black_level) / (this->right_white_level - this->right_black_level);
    }
    this->light->calibrated = 1;
    this->light->dark = (this->light->left < DARK_THRESHOLD) || (this->light->right < DARK_THRESHOLD);
    this->calibration_timer = 0;
    this->calibration_mask = 0;
  }
}
