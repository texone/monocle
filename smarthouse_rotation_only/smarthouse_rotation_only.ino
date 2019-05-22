
#include "Animations.h"
#include "AnimationManager.h"
#include "Comunication.h"
#include "Constants.h"
#include "Motor.h"
#include "Setup.h"
#include "Clock.h"

#include <avr/wdt.h> // watchdog library

Motor motor(STEP_PIN, DIR_PIN, HOME);
Clock clock;

AnimationManager animationManager;
Setup valueSetup;

long loopCount = 0;

unsigned long start_millis;  // used to restart after about 24 hours

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
  start_millis = millis();
  watchdogSetup();
}

void watchdogSetup()
{
  // The wathdog will reset the board after if the watchdog is not reset for over 2 seconds
  cli();        // disable all interrupts
  wdt_reset();  // reset the WDT timer
  /*
  WDTCSR configuration:
  WDIE = 0: Watchdog interrupt disabled
  WDE = 1 :Reset Enable
  WDP3 = 0 :For 2000ms Time-out
  WDP2 = 1 :For 2000ms Time-out
  WDP1 = 1 :For 2000ms Time-out
  WDP0 = 1 :For 2000ms Time-out
  */
  // Enter Watchdog Configuration mode:
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  // Set Watchdog settings:
  WDTCSR = (0<<WDIE) | (1<<WDE) | (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
  sei();
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

void software_reset()
{
  while(1); // infinite loop to trigger watchdog reset
}

void loop() {
  wdt_reset();  // reset watchdog
    //clock.print();
  animationManager.update();
  motor.target(animationManager.steps());
  motor.move();
  loopCount++;

  digitalWrite(LED_PIN, digitalRead(HOME));
  if (digitalRead(HOME) == HIGH) software_reset();
  // if (millis() - start_millis >= (60000)) software_reset();   // reset every one minute
  if (millis() - start_millis >= (60000 * 60)) software_reset();   // reset every one hour
  //  if (millis() - start_millis >= (60000 * 60 * 24)) software_reset();   // reset every 24 hours

  printAnimation(animationManager, &motor);
}
