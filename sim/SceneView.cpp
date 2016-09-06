#include <QPainter>

#include "SceneView.hpp"

SceneView::SceneView(QWidget* parent) :
  QWidget(parent)
{
}

void SceneView::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.fillRect(QRect(0, 0, width() - 1, height() - 1), Qt::blue);
}

void SceneView::post_cycle(const SimulatorCycleBundle& bundle)
{
  robot_pose_ = bundle.robot_pose;
}
