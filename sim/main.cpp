#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include "Simulator.hpp"

#define SIMULATION_USLEEP 50000

int main(int argc, char* argv[])
{
  Simulator simulator;
  if (argc != 2) {
    std::cout << "Usage: line <world>\n";
    return EXIT_FAILURE;
  }
  try {
    simulator.load_environment(argv[1]);
  } catch (const std::exception& e) {
    std::cout << "Exception in Simulator::load_environment(" << argv[1] << "): " << e.what() << '\n';
    return EXIT_FAILURE;
  }
  while (1) {
    DEBUG_OUTPUT("\n-+~+- New cycle -+~+-\n");
    simulator.cycle();
    usleep(SIMULATION_USLEEP);
  }
}
