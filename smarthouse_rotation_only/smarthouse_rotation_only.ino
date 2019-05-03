
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
Clock clock;

AnimationManager animationManager;
Setup valueSetup;

long loopCount = 0;

void setup() {
  // set the digital pin as output:
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  if(DEBUG)Serial.begin(115200);

  //clock.setup();
  valueSetup.setValues(&animationManager, &motor);

  animationManager.setValues(&clock);
  animationManager.setup();
  motor.setup(&animationManager);
}

void printAnimation(AnimationManager theManager, Motor* motor) {
  if(!DEBUG)return;
  if(!PRINT_ANIMATION)return;
  if (loopCount % 100 != 0)return;
  Serial.print(theManager.steps());
  Serial.print(",");
  Serial.print(motor->currentStep);
  Serial.print(",");
  Serial.print(theManager.animationIndex);
  Serial.print(",");
  Serial.print(theManager.inTransition);
  Serial.print(",");
  Serial.print(theManager.updateTime);
  Serial.print(",");
  Serial.print(theManager.time());
  Serial.print(",");
  Serial.println(theManager.value());
}

void loop() {
    //clock.print();
  animationManager.update();
  motor.target(animationManager.steps());
  motor.move();
  loopCount++;

  printAnimation(animationManager, &motor);
}
