
#include "Animations.h"
#include "AnimationManager.h"
#include "Comunication.h"
#include "Constants.h"
#include "Motor.h"
#include "Setup.h"


/*
  Motor drive pulse/dir output to two axes with linear accel/decel
*/

Motor motor(STEP_PIN, DIR_PIN, HOME);
Comunication com;

AnimationManager animationManager;
Setup valueSetup;

void setup() {
  // set the digital pin as output:
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  valueSetup.setValues(&animationManager, &com);

  motor.setup();
  com.setup();
}



void loop() {
  animationManager.update();
  com.print(animationManager);

  //Serial.println("texone");
}
