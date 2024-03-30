#include "main.h"

// ROBOT TEST CONFIG
// Left Drive Motors
pros::Motor leftFrontMtr(20, pros::E_MOTOR_GEARSET_06, true);
pros::Motor leftMidMtr(18, pros::E_MOTOR_GEARSET_06, true);
pros::Motor leftBtmMtr(17, pros::E_MOTOR_GEARSET_06, true);

// Right Drive Motors
pros::Motor rightFrontMtr(13, pros::E_MOTOR_GEARSET_06, false);
pros::Motor rightMidMtr(11, pros::E_MOTOR_GEARSET_06, false);
pros::Motor rightBackMtr(12, pros::E_MOTOR_GEARSET_06, false);

// Motor Groups (Drive)
pros::Motor_Group leftDrive({leftFrontMtr, leftMidMtr, leftBtmMtr});
pros::Motor_Group rightDrive({rightFrontMtr, rightMidMtr, rightBackMtr});

// // Tracking Sensors
// pros::Rotation rot1(18, true);
// pros::Rotation rot2(15, false);
// pros::Rotation rot3(21, true);

// essentials::TrackingWheel Wheel1(&rot1, 2, -0.5);   // Para1
// essentials::TrackingWheel Wheel2(&rot2, 2, -2.5);   // Perp1 (Mid)
// essentials::TrackingWheel Wheel3(&rot3, 2, -6.25);  // Perp2 (Rear)

pros::IMU inertial(16);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() { pros::lcd::initialize(); }

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous
 * selector on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() { std::cout << "Competition Init" << std::endl; }

/**
 * Runs the user autonomous code. This function will be started in its own
 * task with the default priority and stack size whenever the robot is enabled
 * via the Field Management System or the VEX Competition Switch in the
 * autonomous mode. Alternatively, this function may be called in initialize
 * or opcontrol for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start
 * it from where it left off.
 */
void autonomous() { std::cout << "Start of Auton" << std::endl; }

/**
 * Runs the operator control code. This function will be started in its own
 * task with the default priority and stack size whenever the robot is enabled
 * via the Field Management System or the VEX Competition Switch in the
 * operator control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart
 * the task, not resume it from where it left off.
 */
void opcontrol()
{
  std::cout << "Start of Driver" << std::endl;
  lib2131::trackingWheel TW(&rightDrive, 2.75, 5.375, 450);
  lib2131::trackingWheel TW2(&leftDrive, lib2131::WheelTypes::Small_Omni,
                             -5.375, 450);

  TW.reset();
  TW2.reset();

  lib2131::odom::sensors = {&TW, &TW2, nullptr, nullptr};
  lib2131::odom::init();

  while (1)
  {
    pros::delay(10);
  }
}
