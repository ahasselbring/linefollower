#pragma once

#include <string>

#include <QMetaType>

#include "Types.hpp"

struct SimulatorCycleBundle {
  /// the pose of the robot after the cycle
  Pose2D robot_pose;
  /// the debug output collected during the cycle
  std::string debug_output;
};

Q_DECLARE_METATYPE(SimulatorCycleBundle)
