#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "Environment.hpp"

void Environment::load(const std::string& path, SimulatorLoadBundle& bundle)
{
  std::ifstream f(path);
  if (!f.is_open()) {
    throw std::runtime_error("Could not open environment file!");
  }
  std::lock_guard<std::mutex> lg(mutex_);
  // TODO: error handling
  f >> initial_pose_.position.x >> initial_pose_.position.y >> initial_pose_.heading;
  unsigned int number_of_lines = 0;
  f >> number_of_lines;
  lines_.clear();
  lines_.reserve(number_of_lines);
  for (unsigned int i = 0; i < number_of_lines; i++) {
    Line2D line;
    f >> line.p1.x >> line.p1.y >> line.p2.x >> line.p2.y;
    lines_.push_back(line);
  }
  f.close();
  bundle.initial_pose = initial_pose_;
  bundle.lines = lines_;
  bundle.line_width = line_width_;
}

float Environment::get_brightness(const Point2D& position) const
{
  float brightness = 1;
  std::lock_guard<std::mutex> lg(mutex_);
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

const Pose2D& Environment::get_initial_pose() const
{
  std::lock_guard<std::mutex> lg(mutex_);
  return initial_pose_;
}
