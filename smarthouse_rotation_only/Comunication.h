



#ifndef __Comunication__
#define __Comunication__


#include "Comunication.h"
#include "Constants.h"
#include "Clock.h"


void print(Clock theClock) {
  if (!DEBUG)return;
  if (!theClock.foundClock)Serial.println("Couldn't find RTC");
  if (theClock.lostPower)Serial.println("RTC lost power, lets set the time!");
}



#endif
