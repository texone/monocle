#ifndef __AnimationManager__
#define __AnimationManager__

#include "Animations.h"
#include "Constants.h"
#include "Math.h"

enum AnimationMode {
  PREVIEW,
  CYCLE,
  RANDOM
};

class AnimationManager{
  public:
    long lastMicros = -1;
    double updateTime;

    AnimationMode mode = PREVIEW;

    AbstractAnimation* animations[7];
    
    double propabilitySum = 0;
    double propabilitySums[7];

    BaseStillAnimation baseStill;
    RandomStillAnimation randomStill;
    JitterStillAnimation jitterStill;
    RandomMoveAnimation randomMove;
    JitterMoveAnimation jitterMove;
    FullRollAnimation fullRoll;
    RandomRollAnimation randomRoll;

    TransitionAnimation transition;
    
    AbstractAnimation* currentAnimation;
    AbstractAnimation* nextAnimation;

    bool inTransition = true;
    
    AnimationManager(){
      animations[BASE_STILL] = &baseStill;
      animations[RANDOM_STILL] = &randomStill;
      animations[JITTER_STILL] = &jitterStill;
      animations[RANDOM_MOVE] = &randomMove;
      animations[JITTER_MOVE] = &jitterMove;
      animations[FULL_ROLL] = &fullRoll;
      animations[RANDOM_ROLL] = &randomRoll;

      //currentAnimation = animations[BASE_STILL];
    }

    void setup(){
      for(int i = 0; i < 7;i++){
        propabilitySum += animations[i] -> propability;
        propabilitySums[i] = propabilitySum;
      }
    }

    void animation(Animation theAnimation){
      animationIndex = theAnimation;
      animation(animations[theAnimation]);
    }

    void animation(AbstractAnimation *theAnimation){
      nextAnimation = theAnimation;
      if(currentAnimation){
        transition.position0 = currentAnimation->value();
      }else{
        transition.position0 = 1;
      }
      nextAnimation->init();
      transition.position1 = nextAnimation->value();
      transition.init();
      
      currentAnimation = &transition;
      inTransition = true;
    }

    int counter = 0;
    int animationIndex = 0;

    AbstractAnimation* getNextAnimation(){
      switch(mode){
        case PREVIEW:
          return nextAnimation;
        case CYCLE:
          counter++;
          counter %= 7;
          animationIndex = counter;
          return animations[counter];
        case RANDOM:
          double myRandom = dRandom() * propabilitySum;
          for(int i = 0; i < 7;i++){
            if(myRandom < propabilitySums[i]){
              animationIndex = i;
              return animations[i];
            }
          }
          break;
      }
      return nextAnimation;
    }
  
    void update(){
      if(lastMicros <= 0){
        lastMicros = micros();
      }
      
      unsigned long currentMicros = micros(); // take time snapshot
      unsigned long passedTime = currentMicros - lastMicros;
      updateTime = double(passedTime) / 1000000.;
     
      lastMicros = currentMicros;

      currentAnimation->update(updateTime);

      if(currentAnimation->isFinished()){
        if(inTransition){
          inTransition = false;
          currentAnimation = nextAnimation;
        }else{
          animation(getNextAnimation());
        }
      }
    }

    double time(){
      return currentAnimation->time;
    }

    long steps(){
      return currentAnimation->value() * MAX_STEPS;
    }

    double value(){
      return currentAnimation->value();
    }
};
#endif

