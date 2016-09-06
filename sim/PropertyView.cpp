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

void PropertyView::post_cycle(const SimulatorCycleBundle& bundle)
{
  setItem(0, 1, new QTableWidgetItem(QString::number(bundle.robot_pose.position.x)));
  setItem(1, 1, new QTableWidgetItem(QString::number(bundle.robot_pose.position.y)));
  setItem(2, 1, new QTableWidgetItem(QString::number(bundle.robot_pose.heading)));
}
