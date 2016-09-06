#pragma once

#include <vector>

#include <QMetaType>

#include "Types.hpp"

struct SimulatorLoadBundle {
  /// the pose of the robot after resetting
  Pose2D initial_pose;
  /// the lines on the floor
  std::vector<Line2D> lines;
};

Q_DECLARE_METATYPE(SimulatorLoadBundle)
