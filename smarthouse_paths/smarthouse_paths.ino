//--- code ---
/*
  Initial position is at the outer bottom
*/

#include "Constants.h"
#include "Coordinates.h"
#include "Animations.h"
#include "Motor.h"

animFuncPtr animFunction;

Motor motorA(STEP_PIN_A, DIR_PIN_A, HOME_A);
Motor motorZ(STEP_PIN_Z, DIR_PIN_Z, HOME_Z);

int ledState = LOW;             // ledState used to set the LED

double _myProgress;
long myLastMicros;

void setup() {
  motorA.setup();
  motorZ.setup();
  
  Serial.begin(9600);
  // set the digital pin as output:
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  digitalWrite(DIR_PIN_Z, HIGH); //set direction up

  //Z axis up

  // leave element at the outer limit for this test
  // set end positions according to outer limits call setTargetPosition will 
  // correctly initialize start and end
  _myEnd = toCartesian((Polar){0,1});
 
  setTargetPosition();

  animFunction = pendular;

}// void setup end

void loop() {
  
  unsigned long currentMicros = micros(); // take time snapshot
  unsigned long passedTime = currentMicros - myLastMicros;
  _myProgress += double(passedTime) / PERIOD_MICROS;
 
  myLastMicros = currentMicros;

  Cartesian blends = animFunction(_myProgress);

  if(_myProgress > 1){
    _myProgress -= 1;
  }
  
  Polar myPolarCoords = toPolar(blends);

  /** 
   *  this should be the entry point for the motor control
   *  the animationsfunctions should pass valid cartesian coordinates 
   *  sofar there is no interpolation between the different animation functions
   */
 
  
  /*
  Serial.print(degrees(myBlendAngle));
  Serial.print(",");
  Serial.println(myBlendRadius);
 */

    
   
  long myStepA = long(myPolarCoords.a / MAX_ANGLE * MAX_ANGLE_STEPS);
  long myStepZ = long((1 - myPolarCoords.r) * MAX_STEPS_Z);
  
  motorA.move(myStepA);
  motorZ.move(myStepZ);
  
  Serial.print(_myProgress);
  Serial.print(",");
  Serial.print(blends.x);
  Serial.print(",");
  Serial.print(blends.y);
  Serial.print(",");
  Serial.print(myPolarCoords.a);
  Serial.print(",");
  Serial.print(myPolarCoords.r);
  Serial.print(",");
  Serial.print(myStepA);
  Serial.print(",");
  Serial.print(myStepZ);
  Serial.print(",");
  Serial.println(double(passedTime)/ PERIOD_MICROS);
  /*
  Serial.print(",");
  Serial.print(_myYStart);
  Serial.print(",");
  */
 // Serial.println(myStepA + ","+ myStepZ);

 delay(25);
}
