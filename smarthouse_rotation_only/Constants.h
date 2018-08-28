#ifndef __Constants__
#define __Constants__

/**
 * Contains application constants
 */
//Declares
const bool DEBUG = true;

const int LED_PIN =  LED_BUILTIN; // #13=the number of the LED pin

const int STEP_PIN = 10;    //motor1      //  9
const int DIR_PIN = 9;      // motor1     // 10
const int ENABLE = 13;

const int HOME = 8;

// change this value to slow things down or speed them up
const double PERIOD_MICROS = 6. * 1000. * 1000.;

const long MAX_STEPS = 3850;             // 126800
const long MIN_STEP_TIME = 100;            // Top speed motor 1 -MicroS between pulses

const double MAX_ANGLE = radians(175.0 / 2.);
const double MAX_ANGLE_STEPS = MAX_STEPS;

const double MIN_MOVE_TIME = 0.2;

#endif
