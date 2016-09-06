#include <QFileDialog>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QDockWidget>

#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget* parent) :
  QMainWindow(parent),
  scene_view_(new SceneView(this))
{
  setCentralWidget(scene_view_);

  create_actions();
  create_dock_windows();

  setWindowTitle(tr("Simulator"));
  setUnifiedTitleAndToolBarOnMac(true);

  // It seems that a queued connection has to be used when the signal is emitted by another thread.
  connect(&simulation_thread_, SIGNAL(post_cycle(const SimulatorCycleBundle&)), scene_view_, SLOT(post_cycle(const SimulatorCycleBundle&)), Qt::QueuedConnection);
  connect(&simulation_thread_, SIGNAL(post_cycle(const SimulatorCycleBundle&)), debug_view_, SLOT(post_cycle(const SimulatorCycleBundle&)), Qt::QueuedConnection);
  connect(&simulation_thread_, SIGNAL(post_cycle(const SimulatorCycleBundle&)), property_view_, SLOT(post_cycle(const SimulatorCycleBundle&)), Qt::QueuedConnection);
}

void MainWindow::create_actions()
{
  // Add file menu and file toolbar.
  QMenu* file_menu = menuBar()->addMenu(tr("&File"));
  QToolBar* file_tool_bar = addToolBar(tr("File"));
  // Add open action to file menu and file toolbar.
  const QIcon open_icon = QIcon::fromTheme("document-open");
  QAction* open_action = new QAction(open_icon, tr("&Open"), this);
  open_action->setShortcuts(QKeySequence::Open);
  connect(open_action, &QAction::triggered, this, &MainWindow::open);
  file_menu->addAction(open_action);
  file_tool_bar->addAction(open_action);
  // Add quit action to file menu.
  file_menu->addAction(tr("&Quit"), this, &QWidget::close);

  // Add simulation menu and simulation toolbar.
  QMenu* simulation_menu = menuBar()->addMenu(tr("&Simulation"));
  QToolBar* simulation_tool_bar = addToolBar(tr("Simulation"));
  // Add start action to simulation menu and simulation toolbar.
  const QIcon start_icon = QIcon::fromTheme("document-open");
  QAction* start_action = new QAction(start_icon, tr("Start"), this);
  connect(start_action, &QAction::triggered, &simulation_thread_, &SimulationThread::start_simulation);
  simulation_menu->addAction(start_action);
  simulation_tool_bar->addAction(start_action);
  // Add stop action to simulation menu and simulation toolbar.
  const QIcon stop_icon = QIcon::fromTheme("document-open");
  QAction* stop_action = new QAction(stop_icon, tr("Stop"), this);
  connect(stop_action, &QAction::triggered, &simulation_thread_, &SimulationThread::stop_simulation);
  simulation_menu->addAction(stop_action);
  simulation_tool_bar->addAction(stop_action);
  // Add step action to simulation menu and simulation toolbar.
  const QIcon step_icon = QIcon::fromTheme("document-open");
  QAction* step_action = new QAction(step_icon, tr("Step"), this);
  connect(step_action, &QAction::triggered, &simulation_thread_, &SimulationThread::step_simulation);
  simulation_menu->addAction(step_action);
  simulation_tool_bar->addAction(step_action);
  // Add reset action to simulation menu and simulation toolbar.
  const QIcon reset_icon = QIcon::fromTheme("document-open");
  QAction* reset_action = new QAction(reset_icon, tr("Reset"), this);
  connect(reset_action, &QAction::triggered, &simulation_thread_, &SimulationThread::reset_simulation);
  simulation_menu->addAction(reset_action);
  simulation_tool_bar->addAction(reset_action);

  // Add view menu.
  view_menu_ = menuBar()->addMenu(tr("&View"));
}

void MainWindow::create_dock_windows()
{
  QDockWidget* dock = new QDockWidget(tr("Debug Output"), this);
  dock->setAllowedAreas(Qt::BottomDockWidgetArea);
  debug_view_ = new DebugView(dock);
  dock->setWidget(debug_view_);
  addDockWidget(Qt::BottomDockWidgetArea, dock);
  view_menu_->addAction(dock->toggleViewAction());

  dock = new QDockWidget(tr("Properties"), this);
  dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  property_view_ = new PropertyView(dock);
  dock->setWidget(property_view_);
  addDockWidget(Qt::RightDockWidgetArea, dock);
  view_menu_->addAction(dock->toggleViewAction());
}

void MainWindow::open()
{
  QString file_name = QFileDialog::getOpenFileName(this);
  if (!file_name.isEmpty()) {
    simulation_thread_.load_environment(file_name);
  }
}
