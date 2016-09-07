#include <cmath>

#include "Motor.hpp"

Motor::Motor()
{
  reset();
}

void Motor::reset()
{
  omega_ = 0;
}

float Motor::cycle(const float dt, const float voltage)
{
  // TODO: Do this better (e.g. introduce load torque or better discretization).
  omega_ = (J_ * omega_ / dt + Km_ / R_ * voltage) / (J_ / dt + Km_ * Kg_ / R_);
  return omega_;
}
