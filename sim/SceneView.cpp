#include <QPainter>
#include <QWheelEvent>

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
  unsigned int pixel_x = width() / 2 + origin_.x() - robot_pose_.position.y * scale_;
  unsigned int pixel_y = height() / 2 + origin_.y() - robot_pose_.position.x * scale_;
  painter.fillRect(QRect(pixel_x - 5, pixel_y - 5, 10, 10), Qt::red);
}

#ifndef QT_NO_WHEELEVENT
void SceneView::wheelEvent(QWheelEvent* event)
{
  scale_ += event->delta();
  if (scale_ < 50.f) {
    scale_ = 50.f;
  } else if (scale_ > 10000.f) {
    scale_ = 10000.f;
  }
  update();
}
#endif

void SceneView::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    last_drag_position_ = event->pos();
  }
}

void SceneView::mouseMoveEvent(QMouseEvent* event)
{
  if (event->buttons() & Qt::LeftButton) {
    origin_ += event->pos() - last_drag_position_;
    last_drag_position_ = event->pos();
    update();
  }
}

void SceneView::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    origin_ += event->pos() - last_drag_position_;
    last_drag_position_ = QPoint();
    update();
  }
}

void SceneView::post_cycle(const SimulatorCycleBundle& bundle)
{
  robot_pose_ = bundle.robot_pose;
  update();
}
