//--- code ---
/*
  Initial position is at the outer bottom

  
*/
#include "Constants.h"
#include "Coordinates.h"
#include "Animations.h"


animFuncPtr animFunction;

//motor 1 config (A)
boolean dirAstate = true;
boolean stepAstate = false;
unsigned long previousTmA = 0;  // will store last time motor1 was updated

long lastStepA;

//motor 2 config (Z) 
boolean dirZstate = false;
boolean stepZstate = false;
unsigned long previousTmZ = 0;

long lastStepZ;

int Homed = 0;                  // initialize as un-homed
int ledState = LOW;             // ledState used to set the LED
float watchdog;
//boolean ledState = false;

double _myProgress;

Cartesian _myStart = (Cartesian){0,0};
Cartesian _myEnd = (Cartesian){0,0};
  
long myLastMicros;

double easeInOut(double theBlend) {
  return (cos(PI + PI * theBlend) + 1) / 2;
}

Cartesian quickGaze(double theProgress){
  
  
  double myEasedBlend = easeInOut(theProgress);

  Cartesian myResult =  blend(_myStart, _myEnd, myEasedBlend);

  if(theProgress > 1.){
    setTargetPosition();
  }

  return myResult;
}
  
double blend(double theStart, double theEnd, double theBlend){
  return theStart * (1 - theBlend) + theEnd * theBlend;
}

Cartesian blend(Cartesian theStart, Cartesian theEnd, double theBlend){
  return (Cartesian){
    theStart.x * (1 - theBlend) + theEnd.x * theBlend,
    theStart.y * (1 - theBlend) + theEnd.y * theBlend,
  };
}

void setTargetPosition(){
  _myStart = _myEnd;
      
  double myAngle = random(-100000,100000) / 100000. * MAX_ANGLE;
  double myRadius = random(66666,100000) / 100000.;

  _myEnd = toCartesian((Polar){myAngle, myRadius});
}

void setup() {
  Serial.begin(9600);
  // set the digital pin as output:
  pinMode(LED_PIN, OUTPUT);
  pinMode(STEP_PIN_A, OUTPUT);
  pinMode(DIR_PIN_A, OUTPUT);

  pinMode(STEP_PIN_Z, OUTPUT);
  pinMode(DIR_PIN_Z, OUTPUT);

  pinMode(HOME_A, INPUT_PULLUP); // connect: Arduino pin6 & GND
  pinMode(HOME_Z, INPUT_PULLUP); // connect: Arduino pin8 & GND
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
  
  
  dirAstate = myStepA > lastStepA;
  int myStepsA = abs(myStepA - lastStepA);
  lastStepA = myStepA;
  digitalWrite(DIR_PIN_A, dirAstate);
  for (int i = 0; i < myStepsA; i++) {
    stepAstate = !stepAstate;
    digitalWrite(STEP_PIN_A, stepAstate);
  }

  dirZstate = myStepZ > lastStepZ;
  int myStepsZ = abs(myStepZ - lastStepZ);
  lastStepZ = myStepZ;
  digitalWrite(DIR_PIN_Z, dirZstate);
  for (int i = 0; i < myStepsZ; i++) {
    stepZstate = !stepZstate;
    digitalWrite(STEP_PIN_Z, stepZstate);
  }
  /*
  Serial.print(",");
  Serial.print(_myYStart);
  Serial.print(",");
  */
 // Serial.println(myStepA + ","+ myStepZ);

 delay(25);
}
