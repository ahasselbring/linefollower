#include <QPainter>

#include "SceneView.hpp"

SceneView::SceneView(QWidget* parent) :
  QWidget(parent),
  scale_(300.f)
{
}

void SceneView::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.fillRect(QRect(0, 0, width() - 1, height() - 1), Qt::blue);
  unsigned int pixel_x = width() / 2 - (robot_pose_.position.y - center_.y) * scale_;
  unsigned int pixel_y = height() / 2 - (robot_pose_.position.x - center_.x) * scale_;
  painter.fillRect(QRect(pixel_x - 5, pixel_y - 5, 10, 10), Qt::red);
}

void SceneView::post_cycle(const SimulatorCycleBundle& bundle)
{
  robot_pose_ = bundle.robot_pose;
  repaint();
}
