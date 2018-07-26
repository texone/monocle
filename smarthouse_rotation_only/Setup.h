#ifndef __Setup__
#define __Setup__

#include "AnimationManager.h"

class Setup {
  public:
    void setValues(
      AnimationManager* theAnimationManager, 
      Comunication* com,
      Motor* motor
    ) {

      /*
       * make sure debug is false before running on motor this is only for simulation mode
       */
      com->debug = true;
      motor->homing = false;

      //com->debug = false;
      //motor->homing = true;

      // Setup propabilities
      theAnimationManager->baseStill.propability    = 0.1;  // any value bigger than 0
      theAnimationManager->randomStill.propability  = 0.1;  // any value bigger than 0
      theAnimationManager->jitterStill.propability  = 0.1;  // any value bigger than 0
      theAnimationManager->randomMove.propability   = 0.1;  // any value bigger than 0
      theAnimationManager->jitterMove.propability   = 0.1;  // any value bigger than 0
      theAnimationManager->fullRoll.propability     = 0.1;  // any value bigger than 0
      theAnimationManager->randomRoll.propability   = 0.1;  // any value bigger than 0
       
      // Setup BASE_STILL animation
      theAnimationManager->baseStill.minDuration = 3;       // any value bigger than 0
      theAnimationManager->baseStill.maxDuration = 6;       // any value bigger than 0

      // Setup RANDOM_STILL animation
      theAnimationManager->randomStill.minDuration = 3;     // any value bigger than 0
      theAnimationManager->randomStill.maxDuration = 6;     // any value bigger than 0

      // Setup JITTER_STILL animation
      theAnimationManager->jitterStill.minDuration = 10;    // any value bigger than 0
      theAnimationManager->jitterStill.maxDuration = 20;    // any value bigger than 0

      theAnimationManager->jitterStill.jitterAmplitude = 0.1;  // jitter amount relative range 0 to 0.25
      theAnimationManager->jitterStill.jitterFrequency = 0.5;  // movements per second max dependent on amp
      theAnimationManager->jitterStill.jitterFade = 0.1;       // range 0.1 to 0.5 

      // Setup RANDOM_MOVE animation

      theAnimationManager->randomMove.minCycles = 4;          // any value bigger than 0
      theAnimationManager->randomMove.maxCycles = 4;          // any value bigger than 0

      theAnimationManager->randomMove.moveTime = 3;           // value bigger than 3
      theAnimationManager->randomMove.breakTime = 1;        // any value
      
      theAnimationManager->randomMove.moveTimeRandom = 0.2;   // range 0. to 0.5 dependent on moveTime
      theAnimationManager->randomMove.breakTimeRandom = 0.2;  // range 0. to 1. 

      //Setup JITTER_MOVE animation

      theAnimationManager->jitterMove.minCycles = 4;
      theAnimationManager->jitterMove.maxCycles = 4;

      theAnimationManager->jitterMove.jitterAmplitude = 0.1;
      theAnimationManager->jitterMove.jitterFade = 0.5;
      theAnimationManager->jitterMove.jitterFrequency = 0.5;

      theAnimationManager->jitterMove.moveTime = 5;
      theAnimationManager->jitterMove.moveTimeRandom = 0.2;

      theAnimationManager->jitterMove.breakTime = 1;
      theAnimationManager->jitterMove.breakTimeRandom = 0.2;

      // Setup FULL_ROLL animation
      theAnimationManager->fullRoll.frequency = 0.1;            // max value 0.1
      theAnimationManager->fullRoll.minCycles = 3;
      theAnimationManager->fullRoll.maxCycles = 6;

      // Setup RANDOM_ROLL animation
      theAnimationManager->randomRoll.frequency = 0.5;

      theAnimationManager->randomRoll.minCycles = 3;
      theAnimationManager->randomRoll.maxCycles = 6;

      theAnimationManager->randomRoll.minAmplitude = 0.1;
      theAnimationManager->randomRoll.maxAmplitude = 0.2;

      // Setup animation manager
      theAnimationManager->previewAnimation = true;
      theAnimationManager->transition.moveTime = 3;

      /*
       * Choose one of
       * BASE_STILL, 
       * RANDOM_STILL, 
       * JITTER_STILL,
       * RANDOM_MOVE,
       * JITTER_MOVE,
       * FULL_ROLL,
       * RANDOM_ROLL
       */
      theAnimationManager->animation(JITTER_MOVE);
    }
};

#endif
