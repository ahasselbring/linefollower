#include <QPainter>
#include <QWheelEvent>

#include "SceneView.hpp"

SceneView::SceneView(QWidget* parent) :
  QWidget(parent),
  line_width_(0.01),
  robot_pixmap_(":/Images/Robot.png"),
  scale_(300.f)
{
}

void SceneView::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.fillRect(QRect(0, 0, width() - 1, height() - 1), Qt::white);
  QPen pen(Qt::black);
  pen.setWidth(line_width_ * scale_);
  painter.setPen(pen);
  for (auto& line : lines_) {
    // TODO: Check if this is really correct.
    painter.drawLine(global_to_pixel(line.p1), global_to_pixel(line.p2));
  }
  // TODO: Get robot / pixmap dimensions from somewhere else
  QSize pixel_size(0.122 * scale_, 0.117 * scale_);
  QPoint origin_in_pixmap(0.061 * scale_, 0.093 * scale_);
  QRect robot_rect(global_to_pixel(robot_pose_.position) - origin_in_pixmap, pixel_size);
  painter.drawPixmap(robot_rect, robot_pixmap_);
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

QPoint SceneView::global_to_pixel(const Point2D& point) const
{
  // TODO: Something is wrong with zooming, namely the origin isn't scaled.
  return origin_ + QPoint(width() / 2 - point.y * scale_, height() / 2 - point.x * scale_);
}

void SceneView::post_cycle(const SimulatorCycleBundle& bundle)
{
  robot_pose_ = bundle.robot_pose;
  update();
}

void SceneView::post_load(const SimulatorLoadBundle& bundle)
{
  initial_pose_ = bundle.initial_pose;
  lines_ = bundle.lines;
  line_width_ = bundle.line_width;
  update();
}

void SceneView::post_reset()
{
  robot_pose_ = initial_pose_;
  scale_ = 300.f;
  origin_ = QPoint(0, 0);
  update();
}
