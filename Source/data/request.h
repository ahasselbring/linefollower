#pragma once

typedef enum request_type {
  /// nothing happens
  REQUEST_NONE = 0,
  /// initialization phase
  REQUEST_INIT = 1,
  /// keep the robot on track
  REQUEST_DRIVE = 2,
  /// check if the line has ended
  REQUEST_CHECK_FINISHED = 3,
  /// the line has ended
  REQUEST_FINISHED = 4
} request_type_t;

typedef struct request {
  /// the action that shall be executed in this cycle
  request_type_t type;
} request_t;
