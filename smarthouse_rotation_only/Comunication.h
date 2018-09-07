



#ifndef __Comunication__
#define __Comunication__

#include "Animations.h"
#include "AnimationManager.h"
#include "Comunication.h"
#include "Constants.h"
#include "Motor.h"
#include "Clock.h"

/**
   animation class
*/
class Comunication {
  public:

    bool debug = true;

    void setup() {
      if (!debug)return;

      Serial.begin(115200);
    }

    void print(AnimationManager theManager, Motor* motor) {
      if (!debug)return;

      Serial.print(theManager.steps());
      Serial.print(",");
      Serial.print(motor->currentStep);
      Serial.print(",");
      Serial.print(theManager.animationIndex);
      Serial.print(",");
      Serial.print(theManager.inTransition);
      Serial.print(",");
      Serial.print(theManager.updateTime);
      Serial.print(",");
      Serial.println(theManager.time());
    }

    void print(Clock theClock){
      if(!theClock.foundClock)Serial.println("Couldn't find RTC");
      if(theClock.lostPower)Serial.println("RTC lost power, lets set the time!");
    }

    void print(String theString) {
      if (!debug)return;
      Serial.print(theString);
    }
};

#endif
