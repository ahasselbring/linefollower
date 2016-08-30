#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"

// parameters of the robot
#define ROBOT_LINE_SENSOR_X 0.1 // x coordinate of the line sensors relative to the robot origin [m]
#define ROBOT_LINE_SENSOR_Y_ABS 0.01 // absolute value of the y coordinate of the line sensors relative to the robot origin [m]
#define ROBOT_WHEEL_Y_ABS 0.05 // absolute value of the y coordinate of the wheels relative to the robot origin [m]
#define ROBOT_MOTOR_SPEED_FACTOR 0.05 // speed of a wheel at full speed [m/s]

// parameters of the environment
#define ENVIRONMENT_LINE_WIDTH 0.01 // width of the outer black lines [m]

static const float pi_2 = 3.14159 / 2;

static float get_global_line_sensor(const environment_t* environment, const float x, const float y)
{
  float brightness = 1;
  unsigned int i;
  for (i = 0; i < environment->number_of_lines; i++) {
    const struct environment_line* l = &(environment->lines[i]);
    const float y2y1 = l->y2 - l->y1;
    const float x2x1 = l->x2 - l->x1;
    float distance_to_line = fabsf(y2y1 * x - x2x1 * y + l->x2 * l->y1 - l->y2 * l->x1) / sqrtf(y2y1 * y2y1 + x2x1 * x2x1);
    if (distance_to_line < ENVIRONMENT_LINE_WIDTH) {
      brightness -= 0.5 * cosf(pi_2 * distance_to_line / ENVIRONMENT_LINE_WIDTH) + 0.5;
    }
  }
  if (brightness < 0) {
    brightness = 0;
  }
  return brightness;
}

void simulator_get_line_data(const simulator_t* this, unsigned int* left, unsigned int* right)
{
  float s = sinf(this->robot.theta), c = cosf(this->robot.theta);
  float left_x = c * ROBOT_LINE_SENSOR_X - s * ROBOT_LINE_SENSOR_Y_ABS + this->robot.x, left_y = s * ROBOT_LINE_SENSOR_X + c * ROBOT_LINE_SENSOR_Y_ABS + this->robot.y;
  float right_x = c * ROBOT_LINE_SENSOR_X + s * ROBOT_LINE_SENSOR_Y_ABS + this->robot.x, right_y = s * ROBOT_LINE_SENSOR_X - c * ROBOT_LINE_SENSOR_Y_ABS + this->robot.y;
  *left = 800 + get_global_line_sensor(&(this->environment), left_x, left_y) * 200;
  *right = 800 + get_global_line_sensor(&(this->environment), right_x, right_y) * 200;
}

void simulator_set_motor_data(simulator_t* this, const unsigned char left_speed, const unsigned char right_speed,
  const unsigned char left_mode, const unsigned char right_mode)
{
  this->robot.left_speed_request = left_speed;
  this->robot.right_speed_request = right_speed;
  this->robot.left_mode_request = left_mode;
  this->robot.right_mode_request = right_mode;
}

void simulator_cycle(simulator_t* this, const float dt)
{
  float left_speed_real = ROBOT_MOTOR_SPEED_FACTOR * (float)(this->robot.left_speed_request) / 255;
  float right_speed_real = ROBOT_MOTOR_SPEED_FACTOR * (float)(this->robot.right_speed_request) / 255;
  if (this->robot.left_mode_request == 2) {
    left_speed_real = -left_speed_real;
  } else if (this->robot.left_mode_request != 1) {
    left_speed_real = 0;
  }
  if (this->robot.right_mode_request == 2) {
    right_speed_real = -right_speed_real;
  } else if (this->robot.right_mode_request != 1) {
    right_speed_real = 0;
  }
  // TODO: simulate inertia of the motors, friction and some other real physics, i.e. modify *_speed_real
  float s = sinf(this->robot.theta), c = cosf(this->robot.theta), moved_distance = dt * (left_speed_real + right_speed_real) / 2;
  this->robot.x += c * moved_distance;
  this->robot.y += s * moved_distance;
  this->robot.theta += asinf(dt * (right_speed_real - left_speed_real) / (2 * ROBOT_WHEEL_Y_ABS));
}

void simulator_print(const simulator_t* this)
{
  printf("X: %f\nY: %f\nA: %f\n", this->robot.x, this->robot.y, this->robot.theta);
}

void simulator_init(simulator_t* this)
{
  this->robot.x = 0;
  this->robot.y = 0;
  this->robot.theta = 0;
  this->environment.lines = calloc(3, sizeof(struct environment_line));
  if (this->environment.lines == NULL) {
    this->environment.number_of_lines = 0;
    puts("Could not allocate memory for lines.");
    return;
  }
  this->environment.lines[0].x1 = 0;
  this->environment.lines[0].y1 = ENVIRONMENT_LINE_WIDTH;
  this->environment.lines[0].x2 = 1;
  this->environment.lines[0].y2 = ENVIRONMENT_LINE_WIDTH;
  this->environment.lines[1].x1 = 0;
  this->environment.lines[1].y1 = -ENVIRONMENT_LINE_WIDTH;
  this->environment.lines[1].x2 = 1;
  this->environment.lines[1].y2 = -ENVIRONMENT_LINE_WIDTH;
  this->environment.lines[2].x1 = 0;
  this->environment.lines[2].y1 = ENVIRONMENT_LINE_WIDTH;
  this->environment.lines[2].x2 = 0;
  this->environment.lines[2].y2 = -ENVIRONMENT_LINE_WIDTH;
  this->environment.number_of_lines = 3;
}
