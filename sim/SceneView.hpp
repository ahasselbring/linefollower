#pragma once

#include <QWidget>

class SceneView : public QWidget {
  Q_OBJECT
public:
  /**
   * @brief SceneView initializes members
   */
  SceneView();
protected:
  /**
   * @brief paintEvent is called when the widget is painted
   */
  void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
};
