#pragma once

#include <mutex>
#include <string>
#include <vector>

#include "sim/SimulatorLoadBundle.hpp"
#include "sim/Types.hpp"

class Environment {
public:
  /**
   * @brief load loads the environment from a file
   * @param path the path the the file
   * @param bundle a set of data about the new environment
   */
  void load(const std::string& path, SimulatorLoadBundle& bundle);
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
  /// mutex to ensure there are no races
  mutable std::mutex mutex_;
};
