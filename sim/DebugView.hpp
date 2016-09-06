#pragma once

#include <QPlainTextEdit>

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
   * @brief print adds some text to the end of the view
   * @param str the string to add
   */
  void print(const QString& str);
};
