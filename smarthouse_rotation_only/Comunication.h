



#ifndef __Comunication__
#define __Comunication__


#include "Comunication.h"
#include "Constants.h"
#include "Clock.h"
#include <Console.h>

void print() {
  if (!DEBUG)return;

  
}

void println(String theString) {
  if (!DEBUG)return;

  #if defined(ARDUINO_AVR_UNO)
    Serial.println(theString);
  #else
    Console.println(theString);
  #endif
}

void print(Clock theClock) {
  if (!theClock.foundClock)println("Couldn't find RTC");
  if (theClock.lostPower)println("RTC lost power, lets set the time!");
}



#endif
