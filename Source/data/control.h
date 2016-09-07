#pragma once

typedef enum motor_mode {
  /// motor is off
  MOTOR_OFF = 0,
  /// motor turns forwards
  MOTOR_FORWARD = 1,
  /// motor turns backwards
  MOTOR_BACKWARD = 2,
  /// motor is short-circuited
  MOTOR_BREAK = 3
} motor_mode_t;

typedef struct control {
  /// the speed of the left motor in [0,255]
  unsigned char left;
  /// the speed of the right motor in [0,255]
  unsigned char right;
  /// the mode of the left motor
  motor_mode_t left_mode;
  /// the mode of the right motor
  motor_mode_t right_mode;
} control_t;
