#pragma once

typedef struct line {
  /// the brightness of the left light sensor in [0,1023] (uncalibrated)
  unsigned int left;
  /// the brightness of the right light sensor in [0,1023] (uncalibrated)
  unsigned int right;
} line_t;
