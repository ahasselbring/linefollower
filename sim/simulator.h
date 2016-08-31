#pragma once

typedef struct robot {
  /// x coordinate of the robot origin in the global coordinate system [m]
  float x;
  /// y coordinate of the robot origin in the global coordinate system [m]
  float y;
  /// heading of the robot in the global coordinate system [rad]
  float theta;
  /// the requested speed of  the left motor in [0,255]
  unsigned char left_speed_request;
  /// the requested mode for the left motor
  unsigned char right_speed_request;
  /// the requested speed of the right motor in [0,255]
  unsigned char left_mode_request;
  /// the requested mode for the right motor
  unsigned char right_mode_request;
} robot_t;

typedef struct environment {
  /// an array of lines
  struct environment_line {
    /// coordinates of the first point
    float x1, y1;
    /// coordinates of the second point
    float x2, y2;
  } * lines;
  /// the number of lines in the array
  unsigned int number_of_lines;
} environment_t;

typedef struct simulator {
  /// the simulated robot
  robot_t robot;
  /// the simulated environment
  environment_t environment;
} simulator_t;

/**
 * @brief simulator_get_line_data
 * @param this the simulator object
 * @param left is filled with the measurement of the left line sensor
 * @param right is filled with the measurement of the right line sensor
 */
void simulator_get_line_data(const simulator_t* this, unsigned int* left, unsigned int* right);

/**
 * @brief simulator_set_motor_data sets the specified motor commands and executes a simulation cycle
 * @param this the simulator object
 * @param left_speed the requested speed of the left motor in [0,255]
 * @param right_speed the requested speed of the right motor in [0,255]
 * @param left_mode the mode of the left motor
 * @param right_mode the mode of the right motor
 */
void simulator_set_motor_data(simulator_t* this, const unsigned char left_speed, const unsigned char right_speed,
  const unsigned char left_mode, const unsigned char right_mode);

/**
 * @brief simulator_cycle simulates the robot for a given time duration
 * @param this the simulator object
 * @param dt the time to simulate in seconds
 */
void simulator_cycle(simulator_t* this, const float dt);

/**
 * @brief simulator_print prints the state of the simulation
 * @param this the simulator object
 */
void simulator_print(const simulator_t* this);

/**
 * @brief simulator_init initializes the simulation
 * @param this the simulator object
 * @param world path to the world file
 * @return < 0 iff an error occured
 */
int simulator_init(simulator_t* this, const char* world);

/**
 * @brief simulator_destroy frees memory of the simulation
 * @param this the simulator object
 */
void simulator_destroy(simulator_t* this);
