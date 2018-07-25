#ifndef __AnimationManager__
#define __AnimationManager__

#include "Animations.h"
#include "Constants.h"

class AnimationManager{
  public:
    long lastMicros;
    double updateTime;

    bool previewAnimation = true;

    AbstractAnimation* animations[7];

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

      currentAnimation = animations[BASE_STILL];
      animation(BASE_STILL);
    }

    void init(){
      transition.position0 = 0;
      
    }

    void animation(Animation theAnimation){
      animation(animations[theAnimation]);
    }

    void animation(AbstractAnimation *theAnimation){
      nextAnimation = theAnimation;
      
      transition.position0 = currentAnimation->value();
      nextAnimation->init();
      transition.position1 = nextAnimation->value();
      transition.init();
      
      currentAnimation = &transition;
      inTransition = true;
    }
  
    void update(){
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
          animation(nextAnimation);
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

