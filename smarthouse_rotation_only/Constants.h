#ifndef __Constants__
#define __Constants__

/**
 * Contains application constants
 */
//Declares
bool DEBUG = true;

bool PRINT_ANIMATION = true;

const int LED_PIN =  LED_BUILTIN; // #13=the number of the LED pin

const int ENABLE = 11;
const int STEP_PIN = 10;
const int DIR_PIN = 9;
const int HOME = 8;  
// Note the "HOME" pin is really the HLFB pin of the motor drive
// HLFB = High level feedback
// It is set to SERVO ON in the software of the motor drive.
// See Appendix A of the ClearPath User Manual Rev 2.24
// It will be HIGH if the servo drive is power off due to a failure like:
// User Stop
// Motor Enable Conflict
// Max Bus Voltage Exceeded
// Power Event Detected
// Command Speed Too High
// Tracking Error Limit Exceeded
// RMS Torque Limit Exceeded
// Excessive Bus Current
// Excessive Motor Temp
// Momentary Low Bus Voltage
// Old Congig File Version
// Motor Phase Overload
// Hard Stop Gave Way
// Excessive Bus Current
// Commutation Startup Error
// Velocity Set Too High
// RAS Change Rejected
// Speed Too High For RAS
// MagAlign Distance Error
// DSP Watchdog Restart


// change this value to slow things down or speed them up
const double PERIOD_MICROS = 6. * 1000. * 1000.;

const long MAX_STEPS = 3850;             // 126800
const long MIN_STEP_TIME = 100;            // Top speed motor 1 -MicroS between pulses

const double MAX_ANIMATION_ACC = 4;
const double MAX_ANIMATION_VEL = 0.4;

const double MAX_ANGLE = radians(175.0 / 2.);
const double MAX_ANGLE_STEPS = MAX_STEPS;

const double MIN_MOVE_TIME = 0.1;

#endif
