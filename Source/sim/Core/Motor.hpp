#pragma once

class Motor {
public:
  /**
   * @brief Motor initializes members
   */
  Motor();
  /**
   * @brief reset resets the motor
   */
  void reset();
  /**
   * @brief cycle simulates some time of the motor assuming a constant input
   * @param dt the amount of time to simulate
   * @param voltage the input voltage
   * @return the average angular velocity of the motor during the time
   */
  float cycle(const float dt, const float voltage);
private:
  /// motor constant [N*m/A]
  static constexpr float Km_ = 0.00654;
  /// generator constant [V*s/rad]
  static constexpr float Kg_ = 0.006087;
  /// motor armature resistance [Ohm]
  static constexpr float R_ = 16;
  /// motor shaft inertia [kg*m^2]
  static constexpr float J_ = 1.2e-7;
  /// current angular velocity of the motor [rad/s]
  float omega_;
};
