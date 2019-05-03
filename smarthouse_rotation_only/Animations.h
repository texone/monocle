

#ifndef __Animations__
#define __Animations__

#include "Constants.h"
#include "Comunication.h"
#include "Math.h"

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

    double dRandom(){
      return random(0, 100000) / 100000. ;
    }
  

    virtual void init() {
      time = 0;
    }

    virtual void update(double theDeltaTime) {
      time += theDeltaTime;
    }

    virtual double value(double theAmp) {
      return position * theAmp;
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
      if(DEBUG)Serial.println("BASE STILL");
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
      range(1.0);
      position = dRandomRange();
      if(DEBUG)Serial.println("RANDOM STILL");
    }

    double value(double theAmp) {
      return position * theAmp;
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
      if(DEBUG)Serial.println("JITTER STILL");
    }

    virtual double value(double theAmp) {
      double timeBlend = time / duration;
      double blend = min(
        smoothStep(0,jitterFade,timeBlend),
        smoothStep(1,1 - jitterFade,timeBlend)
      );
    
      return (position + cos(time * TWO_PI * jitterFrequency) * jitterAmplitude * blend) * theAmp;
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

    double speed = 0.5;
    double _moveTime = 1;

    double breakTime = 1;
    double breakTimeRandom = 0;
    double _breakTime = 1;
    double delta = 0;

    int minCycles = 3;
    int maxCycles = 6;
    int cycles = 3;
    int cycle = 0;

    void setDelta(){
      delta = position1 - position0;
      _moveTime = max(abs(delta) / speed, MIN_MOVE_TIME);
      _breakTime = breakTime + (dRandom() * 2 - 1) * breakTime * breakTimeRandom;
    }

    void init() {
      AbstractAnimation::init();
      range(1.0);
      position0 = dRandomRange();
      position1 = dRandomRange();
      position = position0;

      setDelta();
      
      cycles = random(3, 6);
      cycle = 0;
      if(DEBUG)Serial.println("RANDOM MOVE");
    }

    void update(double theDeltaTime) {
      AbstractAnimation::update(theDeltaTime);

      if (time < _moveTime) {
        position = position0 + delta * smoothStep(0, 1, time / _moveTime);
      }
      if (time > _moveTime + _breakTime) {
        time = 0;
        cycle++;
        position0 = position1;
        position1 = dRandomRange();
        
        setDelta();
      }
    }

    virtual double value(double theAmp) {
      return position * theAmp;
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
      AbstractAnimation::range(1.0 - jitterAmplitude);
      RandomMoveAnimation::init();
      if(DEBUG)Serial.println("JITTER MOVE");
    }

    void update(double theDeltaTime) {
      RandomMoveAnimation::update(theDeltaTime);
      
      jitterTime += theDeltaTime;
      
    }

    virtual double value(double theAmp) {
      double timeBlend = time / (_moveTime + _breakTime);
      double blend = min(
        cycle == 0 ? smoothStep(0,jitterFade,timeBlend) : 1,
        cycle == cycles ? 1 /*smoothStep(1,1 - jitterFade,timeBlend)*/ : 1
      );
    
      return (position + cos(jitterTime * TWO_PI * jitterFrequency) * jitterAmplitude * blend) * theAmp;
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
      if(DEBUG)Serial.println("FULL ROLL");
    }

    virtual double value(double theAmp) {
      return cos(time * TWO_PI * frequency) * theAmp;
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
    double position0 = 0;
    double position1 = 0;
    double position = 0;

    double speed = 0.5;
    double _moveTime = 1;

    double delta = 0;

    int minCycles = 3;
    int maxCycles = 6;
    int cycles = 3;
    int cycle = 0;

    double minAmp = 0.25;
    
    void init() {
      AbstractAnimation::init();
      position0 = minAmp + dRandom() * (1 - minAmp);
      position1 = -minAmp - dRandom() * (1 - minAmp);
      position = position0;

      delta = position1 - position0;
      _moveTime = max(abs(delta) / speed, MIN_MOVE_TIME);
      cycles = random(3, 6);
      cycle = 0; 
      if(DEBUG)Serial.println("RANDOM ROLL");
    }

    void update(double theDeltaTime) {
      AbstractAnimation::update(theDeltaTime);

      if (time < _moveTime) {
        position = position0 + delta * (cos(time / _moveTime * PI + PI) + 1) / 2;
      }
      if (time >= _moveTime) {
        time = 0;
        cycle++;
        position0 = position1;
        position1 = minAmp + dRandom() * (1 - minAmp);
        if(cycle % 2 == 0)position1 = -position1;
        delta = position1 - position0;
      }
    }

    virtual double value(double theAmp) {
      return position * theAmp;
    }

    virtual bool isFinished() {
      return cycle > cycles;
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

    double moveTime = 20;
    double speed = 0.5;

    void init() {
      AbstractAnimation::init();
      if(DEBUG)Serial.println("TRAnSITION");
    }

    void update(double theDeltaTime) {
      AbstractAnimation::update(theDeltaTime);
      double delta = position1 - position0;
      moveTime = max(abs(delta) / speed, MIN_MOVE_TIME);
     // println(delta);
      
      if (time < moveTime) {
        position = position0 + delta * interpolateCos(time / moveTime);
      }
    }

    virtual double value(double theAmp) {
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
