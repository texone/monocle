#ifndef __Clock__
#define __Clock__

// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

//#include "Comunication.h"

class Clock {
  
  public:
    RTC_DS3231 rtc;
    boolean foundClock;
    boolean lostPower = false;

    uint8_t startHour = 8;
    uint8_t startMinute = 0;

    uint8_t endHour = 20;
    uint8_t endMinute = 0;

  

    void setup() {
      foundClock = rtc.begin();

      if (rtc.lostPower()) {
        lostPower = true;
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
      }
    }

    bool isDay(){
      if(!foundClock)return true;
      
      DateTime now = rtc.now();
      return 
        now.hour() >= startHour &&
        now.hour() < endHour &&
        now.minute() >= startMinute &&
        now.minute() < endMinute;
    }
};
    


#endif

