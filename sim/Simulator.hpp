#pragma once

#include <string>
#include <vector>

#include "Types.hpp"

extern "C" {
#include "controller.h"
}

class Environment {
public:
  /**
   * @brief load loads the environment from a file
   * @param path the path the the file
   */
  void load(const std::string& path);
  /**
   * @brief get_brightness computes the brightness on the surface of the ground in [0,1]
   * @param position the position at which the brightness is requested in the global coordinate system
   * @return the brightness in [0,1]
   */
  float get_brightness(const Point2D& position) const;
  /**
   * @brief get_initial_pose returns the initial pose of the robot
   * @return the initial pose of the robot
   */
  const Pose2D& get_initial_pose() const;
private:
  /// the width of a black line [m]
  static constexpr float line_width_ = 0.02;
  /// the initial pose of the robot
  Pose2D initial_pose_;
  /// a list of black lines on a white surface
  std::vector<Line2D> lines_;
};

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
   */
  void cycle(const float dt);
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
  /// the x coordinate of the line sensors in robot coordinates [m]
  static constexpr float line_sensor_x_ = 0.1;
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
};

class Simulator {
public:
  /**
   * @brief Simulator initializes members
   */
  Simulator();
  /**
   * @brief reset resets the state of the simulator
   */
  void reset();
  /**
   * @brief cycle executes a simulation cycle
   */
  void cycle();
  /**
   * @brief load_environment loads the environment from a file
   * @param path the path the the file
   */
  void load_environment(const std::string& path);
private:
  /// the timestep of one simulation cycle
  static constexpr float dt_ = 0.01;
  /// the simulated environment
  Environment environment_;
  /// the simulated robot
  Robot robot_;
};
