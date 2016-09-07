#pragma once

#include "framework.h"
#include "data/line.h"
#include "data/request.h"
#include "data/light.h"

MODULE(line_perception)
  REQUIRES(line)
  REQUIRES(request)
  PROVIDES(light)
  /// the calibrated black level of the left sensor
  unsigned int left_black_level;
  /// the calibrated white level of the left sensor
  unsigned int left_white_level;
  /// the calibrated black level of the right sensor
  unsigned int right_black_level;
  /// the calibrated white level of the right sensor
  unsigned int right_white_level;
  /// number of cycles since the calibration adaption has been performed
  unsigned int calibration_timer;
  /// mask of the thresholds that are already calibrated
  unsigned char calibration_mask;
END_MODULE(line_perception)
