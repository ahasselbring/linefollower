#pragma once

#include <QWidget>

class SceneView : public QWidget {
  Q_OBJECT
public:
  /**
   * @brief SceneView initializes members
   * @param parent the parent widget
   */
  SceneView(QWidget* parent = 0);
protected:
  /**
   * @brief paintEvent is called when the widget is painted
   */
  void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
};
