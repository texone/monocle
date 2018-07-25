# monocle
code and documentation for the monocle project

## summary of the motion limits

### a axis Rotation

40:1 gearing
3200 pulses / revolution

maximum velocity 220 RPM
maximum acceleration 200 RPM

#### z axis Linear

50mm / revolution
3200 pulses / revolution

maximum velocity 1000 mm / s
maximum acceleration 4000 mm / s2

## animation setup

All setup is done in the setup.h file. There are a total of 7 animations that contain different parameters to control.

### propability

the animation manager randomly cycles through the animations with the propability value you can control how often an animation is used the values can have any value important is the ratio of the values to each other so to have all animations equally picked you would use the same propability for every value
```
theAnimationManager->baseStill.propability    = 0.1;
theAnimationManager->randomStill.propability  = 0.1;
theAnimationManager->jitterStill.propability  = 0.1;
theAnimationManager->randomMove.propability   = 0.1;
theAnimationManager->jitterMove.propability   = 0.1;
theAnimationManager->fullRoll.propability     = 0.1;
theAnimationManager->randomRoll.propability   = 0.1;
```
To play the baseStill animation half of the overall time you would set the values like this
```
theAnimationManager->baseStill.propability    = 0.6;
theAnimationManager->randomStill.propability  = 0.1;
theAnimationManager->jitterStill.propability  = 0.1;
theAnimationManager->randomMove.propability   = 0.1;
theAnimationManager->jitterMove.propability   = 0.1;
theAnimationManager->fullRoll.propability     = 0.1;
theAnimationManager->randomRoll.propability   = 0.1;
```

### timing

All animations give feedback how long they haved to be played back. There are two different concepts to pass the timing the still animations need a min and a max duration value. The move and roll animations need a min and max cycle value. Everytime the animation manager plays back an animation it sets the duration or cycle value to random value between min and max.

#### timing for still animations

A still animation is played back for a time between min and max duration the min and max duration values are in seconds so to have random durations between 5 and 10 seconds for the base still animation you would use the following setting
```
theAnimationManager->baseStill.minDuration = 5;
theAnimationManager->baseStill.maxDuration = 10;
```
If you want an animation to have the same duration every time for example 10 seconds you set the min and max duration to the same value
```
theAnimationManager->baseStill.minDuration = 10;
theAnimationManager->baseStill.maxDuration = 10;
```
#### timing for move animations

Move animations have a move time value and a break time value the move time defines how long an animation moves the eyes the break time defines how long the eye is resting. With the min and max cycle value you can define how many movements the animation is playing until it is finished. So if you want the random move animation to move for 5 seconds and than rest for 5 seconds you would use the following setup. As you can see the animation does between 3 and 6 cycles meaning it moves to 3 to 6 random positions.
```
theAnimationManager->randomMove.minCycles = 3;
theAnimationManager->randomMove.maxCycles = 6;

theAnimationManager->randomMove.moveTime = 5;
theAnimationManager->randomMove.breakTime = 5;
```






