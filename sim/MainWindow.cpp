#include <QFileDialog>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QDockWidget>

#include "MainWindow.hpp"

MainWindow::MainWindow() :
  QMainWindow(),
  graphics_view_(new QPlainTextEdit)
{
  setCentralWidget(graphics_view_);

  createActions();
  createStatusBar();
  createDockWindows();

  setWindowTitle(tr("Simulator"));
  setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::createActions()
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
  connect(start_action, &QAction::triggered, this, &MainWindow::start);
  simulation_menu->addAction(start_action);
  simulation_tool_bar->addAction(start_action);
  // Add stop action to simulation menu and simulation toolbar.
  const QIcon stop_icon = QIcon::fromTheme("document-open");
  QAction* stop_action = new QAction(stop_icon, tr("Stop"), this);
  connect(stop_action, &QAction::triggered, this, &MainWindow::stop);
  simulation_menu->addAction(stop_action);
  simulation_tool_bar->addAction(stop_action);
  // Add step action to simulation menu and simulation toolbar.
  const QIcon step_icon = QIcon::fromTheme("document-open");
  QAction* step_action = new QAction(step_icon, tr("Step"), this);
  connect(step_action, &QAction::triggered, this, &MainWindow::step);
  simulation_menu->addAction(step_action);
  simulation_tool_bar->addAction(step_action);
  // Add reset action to simulation menu and simulation toolbar.
  const QIcon reset_icon = QIcon::fromTheme("document-open");
  QAction* reset_action = new QAction(reset_icon, tr("Reset"), this);
  connect(reset_action, &QAction::triggered, this, &MainWindow::reset);
  simulation_menu->addAction(reset_action);
  simulation_tool_bar->addAction(reset_action);

  // Add view menu.
  view_menu_ = menuBar()->addMenu(tr("&View"));
}

void MainWindow::createStatusBar()
{
  statusBar()->showMessage(tr("Simulation stopped"));
}

void MainWindow::createDockWindows()
{
  QDockWidget* dock = new QDockWidget(tr("Debug Output"), this);
  dock->setAllowedAreas(Qt::BottomDockWidgetArea);
  debug_view_ = new QPlainTextEdit(dock);
  dock->setWidget(debug_view_);
  addDockWidget(Qt::BottomDockWidgetArea, dock);
  view_menu_->addAction(dock->toggleViewAction());

  dock = new QDockWidget(tr("Properties"), this);
  dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  property_view_ = new QPlainTextEdit(dock);
  dock->setWidget(property_view_);
  addDockWidget(Qt::RightDockWidgetArea, dock);
  view_menu_->addAction(dock->toggleViewAction());
}

void MainWindow::open()
{
  QString file_name = QFileDialog::getOpenFileName(this);
  if (!file_name.isEmpty()) {
    simulator_.load_environment(file_name.toStdString());
  }
}

void MainWindow::start()
{
  // TODO: Maybe start a thread?
  statusBar()->showMessage(tr("Simulating"));
}

void MainWindow::stop()
{
  // TODO: Maybe stop a thread?
  statusBar()->showMessage(tr("Simulation stopped"));
}

void MainWindow::step()
{
  simulator_.cycle();
}

void MainWindow::reset()
{
  simulator_.reset();
}
