#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "Simulator.hpp"

class SimulationThread : public QThread {
  Q_OBJECT
public:
  /**
   * @brief SimulationThread initializes members and starts the thread
   */
  SimulationThread();
  /**
   * @brief ~SimulationThread kills the thread
   */
  ~SimulationThread();
  /**
   * @brief debug_output sends a string to the debug output view
   * @param str the string that should be displayed
   */
  static void debug_output(const QString& str);
signals:
  /**
   * @brief add_to_debug is emitted when debug_output is called
   * @param str a string
   */
  void add_to_debug(const QString& str);
public slots:
  /**
   * @brief start_simulation starts continuous simulation
   */
  void start_simulation();
  /**
   * @brief stop_simulation stops continuous simulation
   */
  void stop_simulation();
  /**
   * @brief step_simulation executes a simulation cycle
   */
  void step_simulation();
  /**
   * @brief reset_simulation resets the simulation
   */
  void reset_simulation();
  /**
   * @brief load_environment loads a new environment
   * @param path the path to the environment file
   */
  void load_environment(const QString& path);
protected:
  /**
   * @brief run is executed in the thread and handles the requests
   */
  void run() Q_DECL_OVERRIDE;
private:
  enum RequestType {
    /// do nothing
    IDLE = 0,
    /// run the simulation
    RUN = 1 << 0,
    /// run the simulation for one cycle
    STEP = 1 << 1,
    /// reset the simulator
    RESET = 1 << 2,
    /// load an environment file (implicitly resets the simulator)
    LOAD = 1 << 3,
    /// return from the thread
    TERMINATE = 1 << 4
  };
  /**
   * @brief emit_add_to_debug emits an add_to_debug signal
   * @param str the string that is a parameter to the signal
   */
  void emit_add_to_debug(const QString& str);
  /// the (only) instance of this class
  static SimulationThread* instance_;
  /// the actual simulator
  Simulator simulator_;
  /// a mutex to ensure mutual exclusion for the request
  QMutex mutex_;
  /// a condition variable for waking up the thread on request change
  QWaitCondition condition_;
  /// a mask of the actions that should be executed by the thread
  unsigned int request_;
  /// the path of the file that should be loaded (only valid if LOAD flag is set in request)
  QString load_path_;
};
