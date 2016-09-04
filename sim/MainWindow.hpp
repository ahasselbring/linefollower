#pragma once

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QMenu>

#include "Simulator.hpp"

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  /**
   * @brief MainWindow creates the window and everything else
   */
  MainWindow();
private slots:
  /**
   * @brief open loads a new environment with a file dialog
   */
  void open();
  /**
   * @brief start starts the continuous simulation
   */
  void start();
  /**
   * @brief stop stops the continuous simulation
   */
  void stop();
  /**
   * @brief step executes one simulation cycle
   */
  void step();
  /**
   * @brief reset resets the simulator
   */
  void reset();
private:
  /**
   * @brief createActions creates the menus and toolbars
   */
  void createActions();
  /**
   * @brief createStatusBar creates the status bar
   */
  void createStatusBar();
  /**
   * @brief createDockWindows creates the debug and property view
   */
  void createDockWindows();
  /// a view with a 2D graphical representation of the scene
  QPlainTextEdit* graphics_view_;
  /// a view where debug messages from the modules are printed
  QPlainTextEdit* debug_view_;
  /// a view where properties of the scene can be inspected and edited
  QPlainTextEdit* property_view_;
  /// the menu of the views
  QMenu* view_menu_;
  /// the actual simulator
  Simulator simulator_;
};
