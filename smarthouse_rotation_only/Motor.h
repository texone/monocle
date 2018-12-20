#ifndef __Motor__
#define __Motor__

class Motor {
  public:
    const byte stepPin;
    const byte dirPin;
    const byte homePin;

    boolean dirState = true;
    boolean stepState = false;
    boolean homing = true;
    unsigned long previousTm = 0;  // will store last time motor1 was updated

    long currentStep;

  
    Motor(byte theStepPin, byte theDirPin, byte theHomePin) :
      stepPin(theStepPin),
      dirPin(theDirPin),
      homePin(theHomePin)
    {}


    void setup() {
      pinMode(stepPin, OUTPUT);
      pinMode(dirPin, OUTPUT);
      pinMode(ENABLE, OUTPUT);
      pinMode(homePin, INPUT_PULLUP); // connect: Arduino pin6 & GND

      homeCycle();
    }

    void homeCycle() {
      //enable the motor to be ready after homing
      digitalWrite(ENABLE, HIGH);

      //while(!digitalRead(homePin)) {
      while (digitalRead(homePin) == !LOW && homing) {
        //wait for the home signal to activate, (motor; All-Systems-Go actually)
        delay(20);
      }

      currentStep = MAX_STEPS;
    }

    long targetStep;

    void target(long theStep) {
      targetStep = theStep;
    }

    unsigned long previousTime = 0;  // will store last time motor1 was updated

    void move() {
      unsigned long currentMillis = micros(); // take time snapshot

      if (currentMillis - previousTime >= MIN_STEP_TIME) {
        previousTime = currentMillis;

        long myStepDif = targetStep - currentStep;

        if (myStepDif != 0) {
          dirState = myStepDif < 0;
          stepState = !stepState; //  now flip the state of step output = take a step
          if (dirState) {
            currentStep--;
          } else {
            currentStep++;
          }
        }
      }

      // update all outputs
      digitalWrite(dirPin, dirState); // same as onboard LED
      digitalWrite(stepPin, stepState);
    }

    /*
     * To detect a RMS or other fault on the motor, just constantly monitor the HOME (Boolean) input.
     * On faults this input goes high (or opposite just like when homing).
     * You can then turn off ENABLE, sleep(3000) then turn ENABLE on again to initiate the homing sequence.
     * Your program will have to start from the beginning, first wait for the home to complete than run as normal.
     */
    void checkFault(){
      //  To detect a RMS or other fault on the motor constantly monitor the HOME (Boolean) input.
      // On faults this input goes high (or opposite just like when homing).
      if(digitalRead(homePin) == LOW)return;

      // You can then turn off ENABLE
      digitalWrite(ENABLE, LOW);

      delay(3000);

      //enable the motor to be ready after homing
      homeCycle();
    }

    void update(){
      move();
    }

};

#endif
