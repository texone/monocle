#ifndef __Motor__
#define __Motor__

class Motor{
  
  const byte stepPin;
  const byte dirPin;
  const byte homePin;

  boolean dirState = true;
  boolean stepState = false;
  unsigned long previousTm = 0;  // will store last time motor1 was updated

  long lastStep;
  
  public:
    Motor(byte theStepPin, byte theDirPin, byte theHomePin) :
      stepPin(theStepPin),
      dirPin(theDirPin),
      homePin(theHomePin)
    {
    }

    void setup() {
      pinMode(stepPin, OUTPUT);
      pinMode(dirPin, OUTPUT);
      pinMode(homePin, INPUT_PULLUP);
    }

    void move(long theStep){
      dirState = theStep > lastStep;
      int mySteps = abs(theStep - lastStep);
      lastStep = theStep;
      digitalWrite(dirPin, dirState);
      
      for (int i = 0; i < mySteps; i++) {
        stepState = !stepState;
        digitalWrite(stepPin, stepState);
      }
    }
    
};

#endif
