
#include "Animations.h"
#include "AnimationManager.h"
#include "Comunication.h"
#include "Constants.h"
#include "Motor.h"
#include "Setup.h"
#include "Clock.h"


/*
  Motor drive pulse/dir output to two axes with linear accel/decel
*/

Motor motor(STEP_PIN, DIR_PIN, HOME);
Comunication com;
Clock clock;

AnimationManager animationManager;
Setup valueSetup;

long loopCount = 0;

void setup() { 
  // set the digital pin as output:
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  com.setup();

  valueSetup.setValues(&animationManager, &com, &motor);

  animationManager.setValues(&clock);
  animationManager.setup();
  animationManager.daySetup = setDayValues;
  animationManager.nightSetup = setNightValues;
  motor.setup();
}



void loop() {
  animationManager.update();
  motor.target(animationManager.steps());
  motor.move();
  loopCount++;
  if(loopCount % 10 == 0)com.print(animationManager, &motor);
  //Serial.println("texone");
}
