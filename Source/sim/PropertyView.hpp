#pragma once

#include <QTableWidget>

#include "SimulatorCycleBundle.hpp"
#include "SimulatorLoadBundle.hpp"

class PropertyView : public QTableWidget {
  Q_OBJECT
public:
  /**
   * @brief PropertyView initializes members
   * @param parent the parent widget
   */
  PropertyView(QWidget* parent = 0);
public slots:
  /**
   * @brief post_cycle updates the properties
   * @param bundle a set of data about the state of the simulation
   */
  void post_cycle(const SimulatorCycleBundle& bundle);
  /**
   * @brief post_load stores the initial pose for later use
   * @param bundle a set of data about the new environment
   */
  void post_load(const SimulatorLoadBundle& bundle);
  /**
   * @brief post_reset resets the robot pose
   */
  void post_reset();
private:
  /**
   * @brief set_pose sets the table to view a pose
   * @param pose the pose that should be displayed
   */
  void set_pose(const Pose2D& pose);
  /// the initial robot pose
  Pose2D initial_pose_;
};
