# monocle
code and documentation for the monocle project

## install and run the arduino program

To use the program you need the arduino [ide](https://www.arduino.cc/en/Main/Software) and an arduino. You can directly download the complete project from github [here](https://github.com/texone/monocle/archive/master.zip).

For help to get started look [here](https://www.arduino.cc/en/Guide/HomePage) for the software and [here](https://www.arduino.cc/en/Guide/ArduinoUno) how to connect the arduino.

To start the program load the smarthouse_rotation_only project located in the downloaded folder into the arduino ide using file -> open. Look into the [setup.h](https://github.com/texone/monocle/blob/master/smarthouse_rotation_only/Setup.h) tab and locate the lines
```
com->debug = true;
motor->homing = false;
```
Make sure debug is true and homing false for simulation, debug is false and homing true to run the installation.

There are three animation modes for testing and installation
* **PREVIEW** plays only the defined animation 
* **CYCLE** plays all animations one after another and than starts with the first again
* **RANDOM** plays all animations randomly according to defined propability values

locate the following line in the setup.h tab to set the mode
```
theAnimationManager->mode = RANDOM;
```

now upload the program to the arduino with the upload button at the top.

## install and run the visualizer

The visualizer is a processing application to start it you need processing which you can download [here](https://processing.org/download/). For more information on the processing ide check [here](https://processing.org/reference/environment/).

Once you have processing installed open the visualizer project located in github download inside the smarthouse_visualizer folder using file -> open. Start the application using the start button at the top of the ide. To let the visualizer comunicate with the arduino you need to open the right serial port. At start the visualizer prints out the port available in the console like this
```
/dev/tty.Bluetooth-Incoming-Port
/dev/tty.MacBookPro2-Bluetooth-I
/dev/tty.usbmodem1411
```
In this case /dev/tty.usbmodem1411 would be the correct device. Locate the line
```
myPort = new Serial(this, "/dev/cu.usbmodem1411", 115200);
```
in the code replace /dev/cu.usbmodem1411 with your correct portname. Now the application should be able to connect to the arduino.

If the arduino runs in debug mode it sends the target position of the animation manager and the real position of the motor over the serial port.

<img src="https://github.com/texone/monocle/blob/master/animations/visualizer.png">

As you can see in the screenshot the visualizer translate the motordata send over the serial output into a simulation of the eye. This allows to tweak the animation parameters of the arduino code without the installation. In the screenshot you can also see a red and a green curve. The red curve shows the target position of the animation the green curve shows the current position of the motor. You need to setup the parameters so that the two curve are on top of each other otherwise the animation runs to fast and the motor can not follow.

Be aware that you have to stop the visualizer if you want to upload the arduino code because it block the serial port that is needed to upload the program to the arduino.

There are three keyshots to control the software
* **D** shows and hides the curves
* **S** starts and ends screencapture
* **F** exports the current frame

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

#### timing between animations

Because animations can start with any position there is a transition movement needed to switch from one to the next animation, the duration of this transition can be defined like this
```
theAnimationManager->transition.moveTime = 3;
```

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

Move animations have a move time value and a break time value the move time defines how long an animation moves the eye, the break time defines how long the eye is resting. With the min and max cycle value you can define how many movements the animation is playing until it is finished. So if you want the random move animation to move for 5 seconds and than rest for 5 seconds you would use the following setup. As you can see the animation does between 3 and 6 cycles meaning it moves to 3 to 6 random positions.
```
theAnimationManager->randomMove.minCycles = 3;
theAnimationManager->randomMove.maxCycles = 6;

theAnimationManager->randomMove.moveTime = 5;
theAnimationManager->randomMove.breakTime = 5;
```
It is also possible to vary the move and brake time using random factors. The random factor can range from 0 to 1 0 means no random 1 means that the actual value is between 0 and double the time. For the move time you need to ensure that there is enough time for the motor to move so a random factor of 1 should not be used as the motor can not move in zero time. Here is a setting icluding random factors.
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

## animations

This is a brief description of the individual animations and a list of its parameters. 

### base still animation

<img src="https://github.com/texone/monocle/blob/master/animations/01_base_still.gif" width="300" height="300">

Lets the eye rest at 6 o'clock position 

* **propability** propability with which the animation is picked 
* **minDuration** minimum time the animation is played in seconds
* **maxDuration** maximum time the animation is played in seconds

### random still animation

<img src="https://github.com/texone/monocle/blob/master/animations/02_random_still.gif" width="300" height="300">

Lets the eye rest at a random position between 3 and 9 o' clock

* **propability** propability with which the animation is picked 
* **minDuration** minimum time the animation is played in seconds
* **maxDuration** maximum time the animation is played in seconds

### jitter still animation

<img src="https://github.com/texone/monocle/blob/master/animations/03_jitter_still.gif" width="300" height="300">

Lets the eye jitter at a random position between 3 and 9 o' clock

* **propability** propability with which the animation is picked 
* **minDuration** minimum time the animation is played in seconds
* **maxDuration** maximum time the animation is played in seconds
* **jitterAmplitude** amount of jitter relative to motion range
* **jitterFrequency** cycles of jitter per second
* **jitterFade**  time of jitter fade in and out relative to the animation duration

### random move animation

<img src="https://github.com/texone/monocle/blob/master/animations/04_random_move.gif" width="300" height="300">

Lets the eye move between random positions between 3 and 9 o' clock

* **propability** propability with which the animation is picked 
* **minCycles** minimum cycles the animation is played back
* **maxCycles** maximum cycles the animation is played back
* **moveTime** defines how long an animation moves the eye
* **breakTime** defines how long the eye is resting
* **moveTimeRandom** random factor to vary the move time
* **breakTimeRandom** random factor to vary the break time

### jitter move animation

<img src="https://github.com/texone/monocle/blob/master/animations/03_jitter_still.gif" width="300" height="300">

Lets the eye jittery move between random positions between 3 and 9 o' clock

* **propability** propability with which the animation is picked 
* **minCycles** minimum cycles the animation is played back
* **maxCycles** maximum cycles the animation is played back
* **moveTime** defines how long an animation moves the eye
* **breakTime** defines how long the eye is resting
* **moveTimeRandom** random factor to vary the move time
* **breakTimeRandom** random factor to vary the break time
* **jitterAmplitude** amount of jitter relative to motion range
* **jitterFrequency** cycles of jitter per second
* **jitterFade**  time of jitter fade in and out relative to the animation duration

### full roll animation

<img src="https://github.com/texone/monocle/blob/master/animations/05_full_roll.gif" width="300" height="300">

Lets the eye roll between 3 and 9 o' clock

* **propability** propability with which the animation is picked 
* **minCycles** minimum cycles the animation is played back
* **maxCycles** maximum cycles the animation is played back
* **frequency** cycles of per second

### random roll animation

<img src="https://github.com/texone/monocle/blob/master/animations/06_random_roll.gif" width="300" height="300">

Lets the eye roll between 3 and 9 o' clock

* **propability** propability with which the animation is picked 
* **minCycles** minimum cycles the animation is played back
* **maxCycles** maximum cycles the animation is played back
* **frequency** cycles of per second
* **minAmplitude** minimum amplitude relative to motion range
* **maxAmplitude** maximum amplitude relative to motion range


