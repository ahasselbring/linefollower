#pragma once

#include <mutex>
#include <string>

extern "C" {
#include "controller.h"
}

#include "sim/SimulatorCycleBundle.hpp"
#include "sim/Types.hpp"

#include "Environment.hpp"

class Robot {
public:
  /**
   * @brief Robot initializes members and starts the robot control program
   * @param environment a reference to the environment of the robot
   */
  Robot(const Environment& environment);
  /**
   * @brief ~Robot deinitializes the object
   */
  ~Robot();
  /**
   * @brief reset sets the robot back to its initial state
   */
  void reset();
  /**
   * @brief cycle executes a simulation cycle of the robot including the RCP
   * @param dt the amount of time per controller cycle
   * @param bundle a set of data about the state of the simulation
   */
  void cycle(const float dt, SimulatorCycleBundle& bundle);
  /**
   * @brief debug_print_wrap is a static wrapper around debug_print
   * @param str the string that should be added to the debug output
   */
  static void debug_print_wrap(const std::string& str);
private:
  typedef motor_mode_t MotorMode;
  /**
   * @brief get_line_data_wrap is a static wrapper around get_line_data
   * @param line the line data type
   */
  static void get_line_data_wrap(line_t* line);
  /**
   * @brief set_motor_data_wrap is a static wrapper around set_motor_data
   * @param control the control data type
   */
  static void set_motor_data_wrap(const control_t* control);
  /**
   * @brief get_line_data fills the argument with data from the simulation
   * @param line the line data type
   */
  void get_line_data(line_t* line);
  /**
   * @brief set_motor_data stores the motor request for the simulation
   * @param control the control data type
   */
  void set_motor_data(const control_t* control);
  /**
   * @brief debug_print adds a string to the debug output
   * @param str the string that should be added
   */
  void debug_print(const std::string& str);
  /// the x coordinate of the line sensors in robot coordinates [m]
  static constexpr float line_sensor_x_ = 0.08;
  /// the absolute value of the y coordinate of the line sensors in robot coordinates [m]
  static constexpr float line_sensor_y_abs_ = 0.01;
  /// the absolute value of the y coordinate of the wheels [m] - the x coordinate is 0 by definition
  static constexpr float wheel_y_abs_ = 0.05;
  /// the speed of a wheel at full speed [m/s]
  static constexpr float motor_speed_factor_ = 0.05;
  /// the black value of the line sensors
  static constexpr float line_sensor_black_value_ = 800;
  /// the white value of the line sensors
  static constexpr float line_sensor_white_value_ = 1000;
  /// the only instance of the robot
  static Robot* instance_;
  /// the environment of the robot
  const Environment& environment_;
  /// the robot control program
  controller_t controller_;
  /// the current pose of the robot in world coordinates [m,rad]
  Pose2D pose_;
  /// the desired speed of the left motor
  unsigned char left_speed_request_;
  /// the desired speed of the right motor
  unsigned char right_speed_request_;
  /// the desired mode of the left motor
  MotorMode left_mode_request_;
  /// the desired mode of the right motor
  MotorMode right_mode_request_;
  /// the debug output from the last cycle
  std::string debug_output_;
  /// mutex to ensure there are no races
  mutable std::mutex mutex_;
};
