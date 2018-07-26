
#include "Constants.h"

#ifndef __Animations__
#define __Animations__

double saturate(double val){
  return max(0,min(val, 1));
}

double smoothStep(double edge0, double edge1, double x) {
    double t = saturate((x - edge0) / (edge1 - edge0));
    return t * t * (3.0 - 2.0 * t);
}

enum Animation {
  BASE_STILL,
  RANDOM_STILL,
  JITTER_STILL,
  RANDOM_MOVE,
  JITTER_MOVE,
  FULL_ROLL,
  RANDOM_ROLL
};

/**
   animation class
*/
class AbstractAnimation {
  public:
    /**
       propability to pic this animation
    */
    double propability = 1;
    double time = 0;
    double position = 0;

    long _range = 100000;

    AbstractAnimation() {

    }

    //~AbstractAnimation() {}

    void range(double theRange) {
      _range = theRange * 100000;
    }

    double dRandomRange() {
      return random(-_range, _range) / 100000. ;
    }

    double dRandom() {
      return random(0, 100000) / 100000. ;
    }

    virtual void init() {
      time = 0;
    }

    virtual void update(double theDeltaTime) {
      time += theDeltaTime;
    }

    virtual double value() {
      return position;
    }

    virtual boolean isFinished() {}
};

/**
   Animation to hold at base position 6h
*/
class BaseStillAnimation : public AbstractAnimation {
  public:
    /**
       duration of the animation
    */
    float minDuration = 10;
    float maxDuration = 10;
    float duration = 10;

    BaseStillAnimation() : AbstractAnimation() {
      position = 0;
    }

    void init() {
      AbstractAnimation::init();
      duration = minDuration + dRandom() * (maxDuration - minDuration);
    }

    virtual boolean isFinished() {
      return time > duration;
    }
};

/**
   Animation hold at random position
*/
class RandomStillAnimation : public BaseStillAnimation {

  public:

    void init() {
      BaseStillAnimation::init();
      position = dRandomRange();
    }

    double value() {
      return position;
    }
};

/**
   jitter at a random position
*/
class JitterStillAnimation : public BaseStillAnimation {
  public:

    double jitterAmplitude = 0.1;
    double jitterFrequency = 1;
    double jitterFade = 0.1;

    void init() {
      BaseStillAnimation::init();
      range(1. - jitterAmplitude);
      position = dRandomRange();
    }

    virtual double value() {
      double timeBlend = time / duration;
      double blend = min(
        smoothStep(0,jitterFade,timeBlend),
        smoothStep(1,1 - jitterFade,timeBlend)
      );
    
      return position + cos(time * TWO_PI * jitterFrequency) * jitterAmplitude * blend;
    }
};

/*
   Fast motion to random position than stop
*/
class RandomMoveAnimation : public AbstractAnimation {
  public:
    double position0 = 0;
    double position1 = 0;
    double position = 0;

    double moveTime = 1;
    double moveTimeRandom = 0;
    double _moveTime = 1;

    double breakTime = 1;
    double breakTimeRandom = 0;
    double _breakTime = 1;

    int minCycles = 3;
    int maxCycles = 6;
    int cycles = 3;
    int cycle = 0;

    void init() {
      AbstractAnimation::init();
      position0 = dRandomRange();
      position1 = dRandomRange();
      position = position0;

      _moveTime = moveTime + (dRandom() * 2 - 1) * moveTime * moveTimeRandom;
      _breakTime = breakTime + (dRandom() * 2 - 1) * breakTime * breakTimeRandom;
      cycles = random(3, 6);
      cycle = 0;
    }

    void update(double theDeltaTime) {
      AbstractAnimation::update(theDeltaTime);

      if (time < _moveTime) {
        double delta = position1 - position0;
        position = position0 + delta * smoothStep(0, 1, time / _moveTime);
      }
      if (time > _moveTime + _breakTime) {
        time = 0;
        cycle++;
        position0 = position1;
        position1 = dRandomRange();
      }
    }

    virtual double value() {
      return position;
    }

    virtual bool isFinished() {
      return cycle > cycles;
    }
};

/**
   jitter while slow movement
*/
class JitterMoveAnimation : public RandomMoveAnimation {
  public:

    double jitterAmplitude = 0.1;
    double jitterFrequency = 1;
    double jitterFade = 0.1;
    double jitterTime = 0;

    void init() {
      AbstractAnimation::range(1. - jitterAmplitude);
      RandomMoveAnimation::init();
    }

    void update(double theDeltaTime) {
      RandomMoveAnimation::update(theDeltaTime);
      jitterTime += theDeltaTime;
    }

    virtual double value() {
      return position + cos(jitterTime * TWO_PI * jitterFrequency) * jitterAmplitude;
    }
};

/*
   slow pendular motion over the complete range
*/
class FullRollAnimation : public AbstractAnimation {
  public:
    double frequency = 1;

    int minCycles = 3;
    int maxCycles = 6;
    int cycles = 3;

    void init() {
      AbstractAnimation::init();
      cycles = random(minCycles, maxCycles);
    }

    virtual double value() {
      return cos(time * TWO_PI * frequency);
    }

    virtual bool isFinished() {
      return time * frequency > cycles;
    }
};

/*
   slow pendular motion over a random range
*/
class RandomRollAnimation : public FullRollAnimation {
  public:
    double maxAmplitude = 0.2;
    double minAmplitude = 0.2;
    double amplitude = 0.2;

    void init() {
      FullRollAnimation::init();
      amplitude = minAmplitude + dRandom() * (maxAmplitude - minAmplitude);
      AbstractAnimation::range(1. - amplitude);
      position = dRandomRange();
    }

    virtual double value() {
      return position + cos(time * TWO_PI * frequency) * amplitude;
    }
};

/*
   Fast motion to random position than stop
*/
class TransitionAnimation : public AbstractAnimation {
  public:
    double position0 = 0;
    double position1 = 0;
    double position = 0;

    double moveTime = 5;

    void init() {
      AbstractAnimation::init();
    }

    void update(double theDeltaTime) {
      AbstractAnimation::update(theDeltaTime);

      if (time < moveTime) {
        double delta = position1 - position0;
        position = position0 + delta * smoothStep(0, 1, time / moveTime);
      }
    }

    virtual double value() {
      return position;
    }

    virtual bool isFinished() {
      return time >= moveTime;
    }
};

class HomeAnimation : public AbstractAnimation {
  public :
    void init() {
    }

    virtual bool isFinished() {
      return false;
    }
};
#endif
