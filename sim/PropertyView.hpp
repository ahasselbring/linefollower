#pragma once

#include <QTableWidget>

#include "SimulatorCycleBundle.hpp"

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
};
