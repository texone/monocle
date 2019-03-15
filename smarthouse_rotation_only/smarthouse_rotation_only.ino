
#include "Animations.h"
#include "AnimationManager.h"
#include "Comunication.h"
#include "Constants.h"
#include "Motor.h"
#include "Setup.h"
#include "Clock.h"
#include <Console.h>


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

#if defined(ARDUINO_AVR_UNO)
  Serial.begin(115200);
#else
  Bridge.begin();
  Console.begin();
#endif
  //clock.setup();
  valueSetup.setValues(&animationManager, &motor);

  animationManager.setValues(&clock);
  animationManager.setup();
  animationManager.daySetup = setDayValues;
  animationManager.nightSetup = setNightValues;
  motor.setup(&animationManager);
}

/*
  long lastMillis = 0;

  long timer = 0;
*/

void printAnimation(AnimationManager theManager, Motor* motor) {
  //if(!PRINT_ANIMATION)return;
  if (loopCount % 1000 != 0)return;
#if defined(ARDUINO_AVR_UNO)
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
#else
  Console.print(theManager.steps());
  Console.print(",");
  Console.print(motor->currentStep);
  Console.print(",");
  Console.print(theManager.animationIndex);
  Console.print(",");
  Console.print(theManager.inTransition);
  Console.print(",");
  Console.print(theManager.updateTime);
  Console.print(",");
  Console.print(theManager.time());
  Console.print(",");
  Console.println(theManager.value());
#endif
}

void loop() {
  /*
    timer += millis() - lastMillis;
    lastMillis = millis();

    if(timer > 20000){
    timer = 0;
    animationManager.reset();
    }*/

  animationManager.update();
  motor.target(animationManager.steps());
  motor.move();
  loopCount++;

  printAnimation(animationManager, &motor);
}
