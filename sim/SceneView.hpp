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
  /**
   * @brief mousePressEvent is called when a mouse button is pressed above the widget
   */
  void mousePressEvent(QMouseEvent*) Q_DECL_OVERRIDE;
  /**
   * @brief mouseMoveEvent is called when the mouse is moved above the widget
   */
  void mouseMoveEvent(QMouseEvent*) Q_DECL_OVERRIDE;
  /**
   * @brief mouseReleaseEvent is called when a mouse button is released above the widget
   */
  void mouseReleaseEvent(QMouseEvent*) Q_DECL_OVERRIDE;
private:
  /// the current pose of the robot
  Pose2D robot_pose_;
  /// the pixel coordinates of the global coordinate system origin relative to the center of the widget
  QPoint origin_;
  /// scale factor: pixel coordinate = scale_ * global coordinate
  float scale_;
  /// the last position of the mouse while dragging, to compute the delta
  QPoint last_drag_position_;
};
