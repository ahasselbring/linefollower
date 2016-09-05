#pragma once

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QMenu>

#include "SceneView.hpp"
#include "SimulationThread.hpp"

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  /**
   * @brief MainWindow creates the window and everything else
   * @param parent the parent widget
   */
  MainWindow(QWidget* parent = 0);
public slots:
  /**
   * @brief add_to_debug adds a string at the end of the debug view
   * @param str the string that shall be added
   */
  void add_to_debug(const QString& str);
private slots:
  /**
   * @brief open loads a new environment with a file dialog
   */
  void open();
private:
  /**
   * @brief create_actions creates the menus and toolbars
   */
  void create_actions();
  /**
   * @brief create_dock_windows creates the debug and property view
   */
  void create_dock_windows();
  /// a view with a 2D graphical representation of the scene
  SceneView* scene_view_;
  /// a view where debug messages from the modules are printed
  QPlainTextEdit* debug_view_;
  /// a view where properties of the scene can be inspected and edited
  QPlainTextEdit* property_view_;
  /// the menu of the views
  QMenu* view_menu_;
  /// the thread in which the simulation happens
  SimulationThread simulation_thread_;
};
