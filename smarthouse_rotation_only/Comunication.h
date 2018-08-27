



#ifndef __Comunication__
#define __Comunication__

#include "Animations.h"
#include "AnimationManager.h"
#include "Comunication.h"
#include "Constants.h"
#include "Motor.h"

/**
 * animation class
 */
class Comunication{
  public: 

    bool debug = true; 
    
    void setup(){
      if(!debug)return;
      
      Serial.begin(115200);
    }

    void print(AnimationManager theManager, Motor* motor){
      if(!debug)return;
      
      Serial.print(theManager.steps());
      Serial.print(",");
      Serial.print(motor->currentStep);
      Serial.print(",");
      Serial.print(theManager.animationIndex);
      Serial.print(",");
      Serial.print(theManager.updateTime);
      Serial.print(",");
      Serial.println(theManager.time());
    }
};

#endif
