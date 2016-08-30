#pragma once

typedef struct state {
  /// the position of the robot relative to the middle of the track in tenth of millimeters (left = positive, right = negative)
  signed int x;
  /// the variance of the position
  unsigned int p;
} state_t;
