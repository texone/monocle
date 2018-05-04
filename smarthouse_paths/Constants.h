#ifndef __Constants__
#define __Constants__

//Declares
const int LED_PIN =  LED_BUILTIN; // #13=the number of the LED pin

const int STEP_PIN_A = 10;    //motor1
const int DIR_PIN_A = 9;      // motor1
const int STEP_PIN_Z = 12;    //motor2
const int DIR_PIN_Z = 11;     // motor2

const int HOME_A = 6;
const int HOME_Z = 8;

// change this value to slow things down or speed them up
const double PERIOD_MICROS = 6. * 1000. * 1000.;

const long MAX_STEPS_A = 60000; 
const long MAX_STEPS_Z = 40000;   

const double MAX_ANGLE = radians(175.0 / 2.);
const double MAX_ANGLE_STEPS = MAX_STEPS_A;

#endif
