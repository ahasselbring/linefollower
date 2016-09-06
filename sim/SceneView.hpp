#pragma once

#include <QWidget>

#include "SimulatorCycleBundle.hpp"

class SceneView : public QWidget {
  Q_OBJECT
public:
  /**
   * @brief SceneView initializes members
   * @param parent the parent widget
   */
  SceneView(QWidget* parent = 0);
public slots:
  /**
   * @brief post_cycle updates the view with the new robot pose
   * @param bundle a set of data about the state of the simulation
   */
  void post_cycle(const SimulatorCycleBundle& bundle);
protected:
  /**
   * @brief paintEvent is called when the widget is painted
   */
  void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
#ifndef QT_NO_WHEELEVENT
  /**
   * @brief wheelEvent is called when the mouse wheel moves above the widget
   */
  void wheelEvent(QWheelEvent*) Q_DECL_OVERRIDE;
#endif
private:
  /// the current pose of the robot
  Pose2D robot_pose_;
  // the global coordinates that represent the center of the view
  Point2D center_;
  // scale factor: pixel coordinate = scale_ * global coordinate
  float scale_;
};
