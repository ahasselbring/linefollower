#pragma once

#include <QPlainTextEdit>

class PropertyView : public QPlainTextEdit {
  Q_OBJECT
public:
  /**
   * @brief PropertyView initializes members
   * @param parent the parent widget
   */
  PropertyView(QWidget* parent = 0);
};
