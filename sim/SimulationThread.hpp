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
   * @param parent the parent object
   */
  SimulationThread(QObject* parent = 0);
  /**
   * @brief ~SimulationThread kills the thread
   */
  ~SimulationThread();
signals:
  /**
   * @brief post_cycle is emitted after a complete cycle has been simulated
   * @param debug_output the debug output of that cycle
   */
  void post_cycle(const QString& debug_output);
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
