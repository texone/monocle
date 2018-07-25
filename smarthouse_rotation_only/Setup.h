#ifndef __Setup__
#define __Setup__

#include "AnimationManager.h"

class Setup {
  public:
    void setValues(AnimationManager* theAnimationManager, Comunication* com) {

      /*
       * make sure debug is false before running on motor this is only for simulation mode
       */
      com->debug = true;

      // Setup propabilities
      theAnimationManager->baseStill.propability    = 0.1;
      theAnimationManager->randomStill.propability  = 0.1;
      theAnimationManager->jitterStill.propability  = 0.1;
      theAnimationManager->randomMove.propability   = 0.1;
      theAnimationManager->jitterMove.propability   = 0.1;
      theAnimationManager->fullRoll.propability     = 0.1;
      theAnimationManager->randomRoll.propability   = 0.1;
       
      // Setup BASE_STILL animation
      theAnimationManager->baseStill.minDuration = 3;
      theAnimationManager->baseStill.maxDuration = 6;

      // Setup RANDOM_STILL animation
      theAnimationManager->randomStill.minDuration = 3;
      theAnimationManager->randomStill.maxDuration = 6;

      /*
       * Setup JITTER_STILL animation
       */

      theAnimationManager->jitterStill.minDuration = 3;
      theAnimationManager->jitterStill.maxDuration = 6;

      theAnimationManager->jitterStill.jitterAmplitude = 0.1; // jitter amount relative 
      theAnimationManager->jitterStill.jitterFrequency = 2;   // movements per second
      theAnimationManager->jitterStill.jitterFade = 0.5;      // range 0.1 to 0.5 

      /*
       * Setup RANDOM_MOVE animation
       */

      theAnimationManager->randomMove.minCycles = 3;
      theAnimationManager->randomMove.maxCycles = 6;

      theAnimationManager->randomMove.moveTime = 5;
      theAnimationManager->randomMove.moveTimeRandom = 0.8;

      theAnimationManager->randomMove.breakTime = 5;
      theAnimationManager->randomMove.breakTimeRandom = 0.8;

      /*
       * Setup JITTER_MOVE animation
       */

      theAnimationManager->jitterMove.minCycles = 3;
      theAnimationManager->jitterMove.maxCycles = 6;

      theAnimationManager->jitterMove.jitterAmplitude = 0.1;
      theAnimationManager->jitterMove.jitterFade = 0.5;
      theAnimationManager->jitterMove.jitterFrequency = 2;

      theAnimationManager->jitterMove.moveTime = 5;
      theAnimationManager->jitterMove.moveTimeRandom = 0.8;

      theAnimationManager->jitterMove.breakTime = 5;
      theAnimationManager->jitterMove.breakTimeRandom = 0.8;

      /*
       * Setup FULL_ROLL animation
       */

      theAnimationManager->fullRoll.frequency = 0.5;

      theAnimationManager->fullRoll.minCycles = 3;
      theAnimationManager->fullRoll.maxCycles = 6;

      /*
       * Setup RANDOM_ROLL animation
       */

      theAnimationManager->randomRoll.frequency = 0.5;

      theAnimationManager->randomRoll.minCycles = 3;
      theAnimationManager->randomRoll.maxCycles = 6;

      theAnimationManager->randomRoll.minAmplitude = 0.1;
      theAnimationManager->randomRoll.maxAmplitude = 0.2;

      /*
       * Setup animation manager
       */
      theAnimationManager->previewAnimation = true;

      theAnimationManager->transition.moveTime = 1;

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
      theAnimationManager->animation(JITTER_STILL);
    }
};

#endif
