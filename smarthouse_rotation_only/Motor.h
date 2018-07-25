#ifndef __Motor__
#define __Motor__

class Motor {

    const byte stepPin;
    const byte dirPin;
    const byte homePin;

    boolean dirState = true;
    boolean stepState = false;
    unsigned long previousTm = 0;  // will store last time motor1 was updated

    long currentStep;

  public:
    Motor(byte theStepPin, byte theDirPin, byte theHomePin) :
      stepPin(theStepPin),
      dirPin(theDirPin),
      homePin(theHomePin)
    {}
      

    void setup(){
      pinMode(STEP_PIN, OUTPUT);
      pinMode(DIR_PIN, OUTPUT);
      pinMode(HOME, INPUT_PULLUP); // connect: Arduino pin6 & GND
    }

    void homeCycle() {
      while(!digitalRead(homePin)) {
        // idle here until home input is detected
        delay(100);
      }
    }

    long targetStep;

    void target(long theStep) {
      targetStep = theStep;
    }

    unsigned long previousTime = 0;  // will store last time motor1 was updated
    const float nA = 100;            // Top speed motor 1 -MicroS between pulses

    void move() {
      unsigned long currentMillis = micros(); // take time snapshot

      if (currentMillis - previousTime >= nA) {
        previousTime = currentMillis;

        long myStepDif = targetStep - currentStep;

        if(myStepDif != 0){
          dirState = myStepDif > 0;
          stepState = !stepState; //  now flip the state of step output = take a step
          if(dirState){
            currentStep++;
          }else{
            currentStep--;
          }
        }
/*
        if (stepAcounter > watchdog + 2000) {
          ledState = digitalRead(LED_PIN);
          digitalWrite(LED_PIN, !digitalRead(ledState)); // flash watchdog
          watchdog = stepAcounter;
        }*/

      }

      // update all outputs
      digitalWrite(dirPin, dirState); // same as onboard LED
      digitalWrite(stepPin, stepState);
    }

};

#endif
