# monocle
code and documentation for the monocle project

## summary of the motion limits

### Rotation

4000 pulses from center to outer range

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
It is also possible to variate the move and brake time using random factors. The random factor can range from 0 to 1 0 means no random 1 means that the actual value is between 0 and double the time. For the move time you need to ensure that there is enough time for the motor to move so a random factor of 1 should not be used as the motor can not move in zero time. Here is a setting icluding random factors.
```
theAnimationManager->randomMove.minCycles = 3;
theAnimationManager->randomMove.maxCycles = 6;

theAnimationManager->randomMove.moveTime = 5;
theAnimationManager->randomMove.breakTime = 5;
      
theAnimationManager->randomMove.moveTimeRandom = 0.2;
theAnimationManager->randomMove.breakTimeRandom = 0.8;
```
With this values the move time will range from 4 to 6 the breaktime from 1 to 9 seconds.

#### timing for roll animations

For the roll animation the duration off one cycle is determined by the the frequency value a frequency of 1 means one cycle per second a frequency of 0.1 means a cycle takes 10 seconds. So to let the full roll animation do an animation between 3 to 6 cycles of 10 second length you would use this setup.
```
theAnimationManager->fullRoll.frequency = 0.1;
theAnimationManager->fullRoll.minCycles = 3;
theAnimationManager->fullRoll.maxCycles = 6;
```
### jitter

The two jitter animations share three parameters jitter amplitude, frequency and fade. Amplitude is the amount of jitter relative to the motion range, so an amplitude of 0.1 means the jitter is 10% of the motion range. Frequncy controls the speed of jitter a frequency of 1 lets the eye move one cycle per second. The fade value controls the fade in and out of the jitter relative to the animation duration so if the duration is 10 sec a fade value of 0.1 will cause a fade in and out of 1 sec. Example jitter settings.
```
theAnimationManager->jitterStill.jitterAmplitude = 0.1;
theAnimationManager->jitterStill.jitterFrequency = 0.5;
theAnimationManager->jitterStill.jitterFade = 0.1;
```

### animations

This is a brief description of the individual animations and a list of its parameters. 

#### base still animation

<img src="https://github.com/texone/monocle/blob/master/animations/01_base_still.gif" width="300" height="300">

Lets the eye rest at 6 o'clock position 

* propability
* minDuration
* maxDuration

#### random still animation

<img src="https://github.com/texone/monocle/blob/master/animations/02_random_still.gif" width="300" height="300">

Lets the eye rest at a random position between 3 and 9 o' clock

* propability
* minDuration
* maxDuration

#### jitter still animation

<img src="https://github.com/texone/monocle/blob/master/animations/03_jitter_still.gif" width="300" height="300">

Lets the eye rest at a random position between 3 and 9 o' clock

* propability
* minDuration
* maxDuration
* jitterAmplitude
* jitterFrequency
* jitterFade



