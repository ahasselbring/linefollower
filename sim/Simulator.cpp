#include <cmath>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "Simulator.hpp"

Robot* Robot::instance_ = nullptr;


void Environment::load(const std::string& path)
{
  std::ifstream f(path);
  if (!f.is_open()) {
    throw std::runtime_error("Could not open environment file!");
  }
  // TODO: error handling
  f >> initial_pose_.position.x >> initial_pose_.position.y >> initial_pose_.heading;
  unsigned int number_of_lines = 0;
  f >> number_of_lines;
  lines_.reserve(number_of_lines);
  for (unsigned int i = 0; i < number_of_lines; i++) {
    Line2D line;
    f >> line.p1.x >> line.p1.y >> line.p2.x >> line.p2.y;
    lines_.push_back(line);
  }
  f.close();
}

float Environment::get_brightness(const Point2D& position) const
{
  float brightness = 1;
  // FIXME: Potential race condition with load.
  for (auto& line : lines_) {
    const float d = line.distance_to_point(position);
    if (d < line_width_) {
      brightness -= 0.5 * std::cos(M_PI * d / line_width_) + 0.5;
      if (brightness < 0) {
        return 0;
      }
    }
  }
  return brightness;
}


Robot::Robot(const Environment& environment) :
  environment_(environment)
{
  // Of course, this is a potential race condition, but there are no concurrent threads here.
  if (instance_ != nullptr) {
    throw std::runtime_error("Robot::instance_ is not NULL but a Robot is constructed!");
  }
  instance_ = this;
  controller_init(&controller_, &Robot::get_line_data_wrap, &Robot::set_motor_data_wrap);
}

Robot::~Robot()
{
  instance_ = nullptr;
}

void Robot::cycle(const float dt)
{
  // Execute a cycle of the RCP
  controller_execute(&controller_);
  // Simulate the dynamics of the robot for a specific time interval.
  float left_speed_real = motor_speed_factor_ * static_cast<float>(left_speed_request_) / 255;
  float right_speed_real = motor_speed_factor_ * static_cast<float>(right_speed_request_) / 255;
  if (left_mode_request_ == MOTOR_BACKWARD) {
    left_speed_real = -left_speed_real;
  } else if (left_mode_request_ != MOTOR_FORWARD) {
    left_speed_real = 0;
  }
  if (right_mode_request_ == MOTOR_BACKWARD) {
    right_speed_real = -right_speed_real;
  } else if (right_mode_request_ != MOTOR_FORWARD) {
    right_speed_real = 0;
  }
  // TODO: simulate inertia of the motors, friction and some other real physics, i.e. modify *_speed_real
  const float s = std::sin(pose_.heading), c = std::cos(pose_.heading);
  const float moved_distance = dt * (left_speed_real + right_speed_real) / 2;
  pose_.position.x += c * moved_distance;
  pose_.position.y += s * moved_distance;
  // TODO: NaN?
  pose_.heading += std::asin(dt * (right_speed_real - left_speed_real) / (2 * wheel_y_abs_));
}

void Robot::get_line_data_wrap(line_t* line)
{
  // Also a potential race.
  if (instance_ == nullptr) {
    throw std::runtime_error("Robot::instance_ is NULL in Robot::get_line_data_wrap!");
  }
  instance_->get_line_data(line);
}

void Robot::set_motor_data_wrap(const control_t* control)
{
  // Also a potential race.
  if (instance_ == nullptr) {
    throw std::runtime_error("Robot::instance_ is NULL in Robot::set_motor_data_wrap!");
  }
  instance_->set_motor_data(control);
}

void Robot::get_line_data(line_t* line)
{
  Point2D left_sensor(pose_ * Point2D(line_sensor_x_, line_sensor_y_abs_));
  Point2D right_sensor(pose_ * Point2D(line_sensor_x_, -line_sensor_y_abs_));
  line->left = line_sensor_black_value_ + environment_.get_brightness(left_sensor) * (line_sensor_white_value_ - line_sensor_black_value_);
  line->right = line_sensor_black_value_ + environment_.get_brightness(right_sensor) * (line_sensor_white_value_ - line_sensor_black_value_);
}

void Robot::set_motor_data(const control_t* control)
{
  left_speed_request_ = control->left;
  right_speed_request_ = control->right;
  left_mode_request_ = control->left_mode;
  right_mode_request_ = control->right_mode;
}


Simulator::Simulator() :
  environment_(),
  robot_(environment_)
{
}

void Simulator::cycle()
{
  robot_.cycle(dt_);
}

void Simulator::load_environment(const std::string& path)
{
  environment_.load(path);
}
