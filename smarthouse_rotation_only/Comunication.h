



#ifndef __Comunication__
#define __Comunication__

#include "Animations.h"
#include "AnimationManager.h"
#include "Comunication.h"
#include "Constants.h"

/**
 * animation class
 */
class Comunication{
  public: 

    bool debug = true;
    
    void setup(){
      if(!debug)return;
      
      Serial.begin(9600);
    }

    void print(AnimationManager theManager){
      if(!debug)return;
      
      Serial.print(theManager.steps());
      Serial.print(",");
      Serial.print(theManager.value());
      Serial.print(",");
      Serial.print(theManager.lastMicros);
      Serial.print(",");
      Serial.print(theManager.updateTime);
      Serial.print(",");
      Serial.println(theManager.time());
    }
};

#endif
