#include "Simulator.hpp"

Simulator::Simulator() :
  environment_(),
  robot_(environment_)
{
}

void Simulator::reset()
{
  robot_.reset();
}

void Simulator::cycle(SimulatorCycleBundle& bundle)
{
  robot_.cycle(dt_, bundle);
}

void Simulator::load_environment(const std::string& path, SimulatorLoadBundle& bundle)
{
  environment_.load(path, bundle);
  robot_.reset();
}
