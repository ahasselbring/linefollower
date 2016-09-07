#include <QHeaderView>

#include "PropertyView.hpp"

PropertyView::PropertyView(QWidget* parent) :
  QTableWidget(parent)
{
  setRowCount(3);
  setColumnCount(2);
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  verticalHeader()->setVisible(false);
  horizontalHeader()->setVisible(false);
  setItem(0, 0, new QTableWidgetItem("x"));
  setItem(1, 0, new QTableWidgetItem("y"));
  setItem(2, 0, new QTableWidgetItem("heading"));
}

void PropertyView::set_pose(const Pose2D& pose)
{
  setItem(0, 1, new QTableWidgetItem(QString::number(pose.position.x)));
  setItem(1, 1, new QTableWidgetItem(QString::number(pose.position.y)));
  setItem(2, 1, new QTableWidgetItem(QString::number(pose.heading)));
}

void PropertyView::post_cycle(const SimulatorCycleBundle& bundle)
{
  set_pose(bundle.robot_pose);
}

void PropertyView::post_load(const SimulatorLoadBundle& bundle)
{
  initial_pose_ = bundle.initial_pose;
}

void PropertyView::post_reset()
{
  set_pose(initial_pose_);
}
