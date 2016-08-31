#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "simulator.h"
#include "controller.h"

#define SIMULATION_DT 0.01
#define SIMULATION_USLEEP 50000

static simulator_t simulator;

static void get_line_data(line_t* line)
{
  simulator_get_line_data(&simulator, &(line->left), &(line->right));
}

static unsigned char convert_motor_mode(motor_mode_t mode)
{
  switch (mode) {
    case MOTOR_FORWARD:
      return 1;
    case MOTOR_BACKWARD:
      return 2;
    case MOTOR_BREAK:
      return 3;
    case MOTOR_OFF:
      return 4;
  }
  return 0;
}

static void set_motor_data(const control_t* control)
{
  simulator_set_motor_data(&simulator, control->left, control->right, convert_motor_mode(control->left_mode), convert_motor_mode(control->right_mode));
}

int main(int argc, char* argv[])
{
  controller_t controller;
  if (argc != 2) {
    puts("Usage: line <world>");
    return EXIT_FAILURE;
  }
  if (simulator_init(&simulator, argv[1]) < 0) {
    return EXIT_FAILURE;
  }
  controller_init(&controller, &get_line_data, &set_motor_data);
  while (1) {
    DEBUG_OUTPUT("\n-+~+- New cycle -+~+-\n");
    controller_execute(&controller);
    simulator_cycle(&simulator, SIMULATION_DT);
    usleep(SIMULATION_USLEEP);
  }
}
