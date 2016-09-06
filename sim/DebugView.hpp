#pragma once

#include <QPlainTextEdit>

#include "SimulatorCycleBundle.hpp"

class DebugView : public QPlainTextEdit {
  Q_OBJECT
public:
  /**
   * @brief DebugView initializes members
   * @param parent the parent widget
   */
  DebugView(QWidget* parent = 0);
public slots:
  /**
   * @brief post_cycle adds the debug output of the bundle to the end of the view
   * @param bundle a set of data about the state of the simulation
   */
  void post_cycle(const SimulatorCycleBundle& bundle);
  /**
   * @brief post_reset clears the text
   */
  void post_reset();
};
