#pragma once

#include <string>

#include "Core/Robot.hpp"
#include "Core/Environment.hpp"
#include "SimulatorCycleBundle.hpp"
#include "SimulatorLoadBundle.hpp"

class Simulator {
public:
  /**
   * @brief Simulator initializes members
   */
  Simulator();
  /**
   * @brief reset resets the state of the simulator
   */
  void reset();
  /**
   * @brief cycle executes a simulation cycle
   * @param bundle a set of data about the state of the simulation
   */
  void cycle(SimulatorCycleBundle& bundle);
  /**
   * @brief load_environment loads the environment from a file
   * @param path the path the the file
   * @param bundle a set of data about the new environment
   */
  void load_environment(const std::string& path, SimulatorLoadBundle& bundle);
private:
  /// the timestep of one simulation cycle
  static constexpr float dt_ = 0.01;
  /// the simulated environment
  Environment environment_;
  /// the simulated robot
  Robot robot_;
};
