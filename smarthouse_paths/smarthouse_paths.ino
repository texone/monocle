//--- code ---
/*
  Initial position is at the outer bottom

  
*/
//Declares
const int LED_PIN =  LED_BUILTIN; // #13=the number of the LED pin

const int STEP_PIN_A = 9;   //motor1
const int DIR_PIN_A = 10;   // motor1
const int STEP_PIN_Z = 11;  //motor2
const int DIR_PIN_Z = 12;   // motor2

const int HOME_A = 6;
const int HOME_Z = 8;

//motor 1 config (A)
const long MAX_STEPS_A = 126800;      //motor 1
long lastStepA;
boolean dirAstate = true;
boolean stepAstate = false;

//motor 2 config (Z)
const long MAX_STEPS_Z = 30000;       //motor 2
long lastStepZ;
boolean dirZstate = true;
boolean stepZstate = false;

int Homed = 0;                  // initialize as un-homed
unsigned long previousTmA = 0;  // will store last time motor1 was updated
unsigned long previousTmZ = 0;  // ... motor2
int ledState = LOW;             // ledState used to set the LED
float watchdog;
//boolean ledState = false;


unsigned long _myStart;

double _myProgress;

double _myXStart = 0;
double _myYStart = 0;
  
double _myXEnd = 0;
double _myYEnd = 0;
  
long myLastMicros;

// change this value to slow things down or speed them up
const double PERIOD_MICROS = 60. * 1000. * 1000.;

const double MAX_ANGLE = radians(175.0 / 2.);
const double MAX_ANGLE_STEPS = MAX_STEPS_A / 2;

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

  digitalWrite(DIR_PIN_Z, LOW); //set direction up

  //Z axis up

  // leave element at the outer limit for this test
  // set end positions according to outer limits call setTargetPosition will 
  // correctly initialize start and end
  _myXEnd = x(0,1);
  _myYEnd = y(0,1);
 
  _myStart = micros();
  setTargetPosition();

}// void setup end


double x(double theAngle, double theRadius){
  return cos( theAngle ) * theRadius;
}
  
double y(double theAngle, double theRadius){
  return sin( theAngle ) * theRadius;
}
  
double angle(double theX, double theY){
  return atan2(theY, theX);
}
  
double radius(double theX, double theY){
  return sqrt(theX * theX + theY * theY);
}
  
double easeInOut(double theBlend) {
  return (cos(PI + PI * theBlend) + 1) / 2;
}
  
double blend(double theStart, double theEnd, double theBlend){
  return theStart * (1 - theBlend) + theEnd * theBlend;
}

void setTargetPosition(){
  _myXStart = _myXEnd;
  _myYStart = _myYEnd;
      
  double myAngle = random(-100000,100000) / 100000. * MAX_ANGLE;
  double myRadius = random(66666,100000) / 100000.;
      
  _myXEnd = x(myAngle, myRadius);
  _myYEnd = y(myAngle, myRadius);
      
  _myProgress = 0;
  myLastMicros = micros() - _myStart;
}

void loop() {
  /* while (Homed < 2){ // check home status for homed ok and estop (maybe)
     //wait in this loop
     Homed=digitalRead(HOME_A);
     Homed = Homed + digitalRead(HOME_Z); // two motor switches at 1 = 2, so ok to go...
    }
  */
  if(_myProgress > 1.){
    setTargetPosition();
  }

  unsigned long currentMicros = micros(); // take time snapshot
  double myEasedBlend = easeInOut(_myProgress);
    
  double myBlendX = blend(_myXStart, _myXEnd, myEasedBlend);
  double myBlendY = blend(_myYStart, _myYEnd, myEasedBlend);
    
  double myBlendAngle = angle(myBlendX, myBlendY);
  double myBlendRadius = radius(myBlendX, myBlendY);

  /*
  Serial.print(degrees(myBlendAngle));
  Serial.print(",");
  Serial.println(myBlendRadius);
  */
    
  long myStepA = long(myBlendAngle / MAX_ANGLE * MAX_ANGLE_STEPS);
  long myStepZ = long((1 - myBlendRadius) * MAX_ANGLE_STEPS);
  //  _myPosition.set(x(myBlendAngle, myRadius * 300), y(myBlendA
  //ngle, myRadius * 300));
/*
   Serial.print(myStepA);
   Serial.print(",");
   Serial.println(myStepZ);
   */
  _myProgress = double((currentMicros - myLastMicros)) / PERIOD_MICROS;
  
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
}
