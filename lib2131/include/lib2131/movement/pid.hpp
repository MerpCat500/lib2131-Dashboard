#pragma once

#include <cmath>

#include "lib2131/movement/timer.hpp"

namespace lib2131
{

/**
 * PID EXIT CONDITIONS
 * @brief Exit Conditions for a PID
 */
struct PID_Exit
{
  /**
   * PID_Exit
   * @param double _small_error : Error that is constituted as small
   * @param double _small_time : Time within a small error before being settled
   * @param double _large_error : Error that is constituted as large
   * @param double _large_time : Time within a large error before being settled
   */

  double _small_error;
  double _small_time;

  double _large_error;
  double _large_time;
};

/**
 * PID CONTROLS
 * @brief Control Values for a PID
 */
struct PID_Controls
{
  /**
   * PID_Controls
   * @param  double kP : P tuning Constant
   * @param  double kI : I tuning Constant
   * @param  double kD : D tuning Constant
   * @param  PID_Exit Exit_Conditions : Exit Conditions For PID
   * @param  bool isVelocity : If PID to control a Velocity Target (Flywheels)
   */

  double kP;
  double kI;
  double kD;

  PID_Exit Exit_Conditions;

  bool isVelocity;
};

/**
 * PID CLASS
 * @brief Class for a PID
 */
class PID
{
 private:
  PID_Controls _Controls;  // Controls
  timer _timer;            // Timer for Settled
  double _last_error;      // Error from last update
  double _last_I;          // Last Integral Error for TBH

  bool _settled;  // Is PID Settled?

  double P, I, D;  // P, I, D Outputs

 public:
  /**
   * PID
   * @brief Constructor for a PID Controller
   * @param PID_Controls Controls : Controls for the PID
   */
  PID(PID_Controls Controls);
  /**
   * PID
   * @brief Constructor for a PID Controller
   * @param PID_Controls Controls : Controls for the PID
   * @param timer Timer : Timer for PID
   */
  PID(PID_Controls Controls, timer Timer);
  /**
   * UPDATE PID
   * @brief Get a PID output
   * @param  double error : Error = Target - Actual
   * @return double      : PID * kPID
   */
  double update(double error);
  /**
   * IS PID SETTLED
   * @brief Returns if PID is settled
   * @return bool : Settled
   */
  bool get_settled();
  /**
   * Returns if PID is a velocity PID
   * @return bool  : is Velocity PID?
   */
  bool get_Type();
  /**
   * RESET PID
   * @brief Resets PID
   */
  void reset();
};

}  // namespace lib2131
