#pragma once

#include "data/line.h"
#include "data/control.h"

#include "module/behavior_control.h"
#include "module/line_perception.h"
#include "module/modeling.h"
#include "module/motor_control.h"

typedef struct controller {
  /// a module that defines the behavior of the robot
  behavior_control_t behavior_control;
  /// a module that provides the surface on the line
  line_perception_t line_perception;
  /// a module that estimates the state
  modeling_t modeling;
  /// a module that controls the motors
  motor_control_t motor_control;
  /// pointer to a function that gets data from the line sensors
  void (*get_line_data)(line_t*);
  /// pointer to a function that sets the motor output
  void (*set_motor_data)(const control_t*);
} controller_t;

/**
 * @brief controller_execute runs the controller for one cycle
 * @param this the controller object
 */
void controller_execute(controller_t* this);

/**
 * @brief controller_init initializes the controller
 * @param this the controller object
 * @param get_line_data a function that gets data from the line sensors
 * @param set_motor_data a function that sets the motor output
 * @return < 0 iff an error occured
 */
int controller_init(controller_t* this, void (*get_line_data)(line_t*), void (*set_motor_data)(const control_t*));
