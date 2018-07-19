//--- code ---
/*
  Motor drive pulse/dir output to two axes with linear accel/decel
*/
//Declares
const int ledPin =  LED_BUILTIN; // #13=the number of the LED pin

const int StepPinA = 9;   //motor1
const int DirPinA = 10;   // motor1
const int StepPinZ = 11;  //motor2
const int DirPinZ = 12;   // motor2

const int homeA = 6;
const int homeZ = 8;

//motor 1 config (A)
float intervalA = 1000;       // interval at which to step (micro seconds)
const float nA = 100;         // Top speed motor 1 -MicroS between pulses
const float aA = 0.1;         // accel rate, motor 1
long stepAcounter = 0;
long maxstepsA = 126800;      //motor 1
boolean dirAstate = true;
boolean stepAstate = false;
boolean stoppingA;

//motor 2 config (Z)
float intervalZ = 1000;       // interval at which to step (micro seconds)
const float nZ = 250;         // Top speed; motor 2 time between pulses
const float aZ = 0.1;         // accel rate, motor 2
long stepZcounter = 0;
long maxstepsZ = 30000;       //motor 2
boolean dirZstate = true;
boolean stepZstate = false;
boolean stoppingZ;

int Homed = 0;                  // initialize as un-homed
unsigned long previousTmA = 0;  // will store last time motor1 was updated
unsigned long previousTmZ = 0;  // ... motor2
int ledState = LOW;             // ledState used to set the LED
float watchdog;
//boolean ledState = false;

void setup() {
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  pinMode(StepPinA, OUTPUT);
  pinMode(DirPinA, OUTPUT);
  pinMode(StepPinZ, OUTPUT);
  pinMode(DirPinZ, OUTPUT);
  pinMode(homeA, INPUT_PULLUP); // connect: Arduino pin6 & GND
  pinMode(homeZ, INPUT_PULLUP); // connect: Arduino pin8 & GND
  digitalWrite(ledPin, HIGH);

  digitalWrite(DirPinZ, LOW); //set direction up
  
  //Z axis up
  for (long nn = 0; nn < (maxstepsZ + 2000); nn ++) { // raise z off the bottom before running...
    digitalWrite(StepPinZ, !digitalRead(StepPinZ)); //step motor 2 upwards before run
    delayMicroseconds(300);//speed limiter
  }
  
  //A axis half turn
  for (long nn = 0; nn < maxstepsA / 2; nn ++) { // raise A to go half off center before running...
    digitalWrite(StepPinA, ! digitalRead(StepPinA));
    delayMicroseconds(100);//speed limiter
  }

  delay(3000);//pause for 3 seconds

}// void setup end

void loop() {
  /* while (Homed < 2){ // check home status for homed ok and estop (maybe)
     //wait in this loop
     Homed=digitalRead(homeA);
     Homed = Homed + digitalRead(homeZ); // two motor switches at 1 = 2, so ok to go...
    }
  */

  unsigned long currentMillis = micros(); // take time snapshot
  //===============================================================================================
  // motor 1

  if (currentMillis - previousTmA >= intervalA) {
    previousTmA = currentMillis;
    
    if (intervalA >= nA) intervalA = intervalA - aA; // increase accel rate; a single line if statement
    
    stepAstate = !stepAstate; //  now flip the state of step output = take a step
    stepAcounter = stepAcounter + 1; //count the step
    
    if (stepAcounter > watchdog + 2000) {
      ledState = digitalRead(ledPin);
      digitalWrite(ledPin, !digitalRead(ledState)); // flash watchdog
      watchdog = stepAcounter;
    }

  }

  //direction
  if (stepAcounter > maxstepsA) {
    dirAstate = !dirAstate; //switch direction
    stepAcounter = 0; //reset step count
    intervalA = 1500; // startup slow
  }

  //===============================================================================================
  // motor 2
  if (currentMillis - previousTmZ >= intervalZ) {
    previousTmZ = currentMillis;
    if (intervalZ >= nZ) intervalZ = intervalZ - aZ; //accel rate, single line if statement
    stepZstate = !stepZstate;
    stepZcounter = stepZcounter + 1;
  }

  //direction
  if (stepZcounter > maxstepsZ) {
    dirZstate = !dirZstate;
    stepZcounter = 0;
    intervalZ = 1500; // startup slow
  }
  //===============================================================================================

  // update all outputs
  digitalWrite(DirPinA, dirAstate); // same as onboard LED
  digitalWrite(StepPinA, stepAstate);
  digitalWrite(DirPinZ, dirZstate);
  digitalWrite(StepPinZ, stepZstate);

}

//-- Code end ---
