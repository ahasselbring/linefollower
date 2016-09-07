#pragma once

typedef struct light {
  /// the brightness of the left light sensor in [0,255] (0=black, 255=white)
  unsigned char left;
  /// the brightness of the right light sensor in [0,255] (0=black, 255=white)
  unsigned char right;
  /// != 0 iff the sensor is calibrated
  unsigned char calibrated;
  /// != 0 iff the sensor measures something dark
  unsigned char dark;
} light_t;
