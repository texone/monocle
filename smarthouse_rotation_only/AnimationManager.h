#ifndef __AnimationManager__
#define __AnimationManager__

#include "Animations.h"
#include "Constants.h"
#include "Math.h"
#include "Filter.h"
#include "Clock.h"


enum AnimationMode {
  PREVIEW,
  CYCLE,
  RANDOM
};

enum TimeMode {
  CLOCK,
  NIGHT,
  DAY
};

class AnimationManager {
  public:
    unsigned long lastMicros = 0;
    double updateTime;
    double amp = 1;

    AnimationMode mode = PREVIEW;
    TimeMode timeMode = CLOCK;

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

    void (*daySetup)(AnimationManager*);
    void (*nightSetup)(AnimationManager*);

    Clock* clock;

    // standard Lowpass, set to the corner frequency
    //FilterTwoPole filterTwoLowpass;                                       // create a two pole Lowpass filter

    double _myCurrentPos = 1;
    double _myAcc = 0;
    double _myVel = 0;

    AnimationManager() {
      animations[BASE_STILL] = &baseStill;
      animations[RANDOM_STILL] = &randomStill;
      animations[JITTER_STILL] = &jitterStill;
      animations[RANDOM_MOVE] = &randomMove;
      animations[JITTER_MOVE] = &jitterMove;
      animations[FULL_ROLL] = &fullRoll;
      animations[RANDOM_ROLL] = &randomRoll;

      //float testFrequency = 2;                     // test signal frequency (Hz)
      // float testAmplitude = 100;                   // test signal amplitude
      //filterTwoLowpass.setAsFilter( LOWPASS_BUTTERWORTH, testFrequency );

      //currentAnimation = animations[BASE_STILL];
    }

    void setValues(Clock* theClock) {
      clock = theClock;
    }

    void setup() {
      for (int i = 0; i < 7; i++) {
        propabilitySum += animations[i] -> propability;
        propabilitySums[i] = propabilitySum;
      }
    }

    void applySetup() {
      /*
      switch (timeMode) {
        case DAY:
          daySetup(this);
          break;
        case NIGHT:
          nightSetup(this);
          break;
        case CLOCK:
          if(clock->isDay()){
            daySetup(this);
          }else{
            nightSetup(this);
          }
          break;
      }
*/
    }

    void animation(Animation theAnimation) {
      animationIndex = theAnimation;
      animation(animations[theAnimation]);
    }

    void animation(AbstractAnimation *theAnimation) {
      nextAnimation = theAnimation;
      if (currentAnimation) {
        transition.position0 = currentAnimation->value(amp);
      } else {
        transition.position0 = 1;
      }
      nextAnimation->init();
      transition.position1 = nextAnimation->value(amp);
      transition.init();

      currentAnimation = &transition;
      inTransition = true;

      applySetup();
    }

    int counter = 0;
    int animationIndex = 0;

    AbstractAnimation* getNextAnimation() {
      switch (mode) {
        case PREVIEW:
          return nextAnimation;
        case CYCLE:
          counter++;
          counter %= 7;
          animationIndex = counter;
          return animations[counter];
        case RANDOM:
          double myRandom = dRandom() * propabilitySum;
          for (int i = 0; i < 7; i++) {
            if (myRandom < propabilitySums[i]) {
              animationIndex = i;
              return animations[i];
            }
          }
          break;
      }
      return nextAnimation;
    }

    void update() {
      if (lastMicros <= 0) {
        lastMicros = micros();
      }

      unsigned long currentMicros = micros(); // take time snapshot
      unsigned long passedTime = currentMicros - lastMicros;
      updateTime = double(passedTime) / 1000000.;

      lastMicros = currentMicros;

      currentAnimation->update(updateTime);

      if (currentAnimation->isFinished()) {
        if (inTransition) {
          inTransition = false;
          currentAnimation = nextAnimation;
        } else {
          animation(getNextAnimation());
        }
      }

      double myTarget = value();

      double myAcc = (myTarget - (_myCurrentPos +  _myVel * 1)) * 100;
      myAcc = constrain(myAcc, -MAX_ANIMATION_ACC, MAX_ANIMATION_ACC);
      _myVel += myAcc * updateTime;
      _myVel = constrain(_myVel, -MAX_ANIMATION_VEL,MAX_ANIMATION_VEL);
      _myCurrentPos += _myVel * updateTime;
    }

    double time() {
      return currentAnimation->time;
    }

    long steps() {
      return _myCurrentPos * MAX_STEPS;
    }

    double value() {
      return currentAnimation->value(amp);//filterTwoLowpass.input();
    }
};
#endif
