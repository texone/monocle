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

    uint8_t restartHour = 5;
    uint8_t restartMinute = 0;

    uint8_t lastMinute = 0;

    char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};



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

    void print() {
      DateTime now = rtc.now();
      
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(" (");
      Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
      Serial.print(") ");
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.println();

      Serial.print(" since midnight 1/1/1970 = ");
      Serial.print(now.unixtime());
      Serial.print("s = ");
      Serial.print(now.unixtime() / 86400L);
      Serial.println("d");
    }

    bool isDay() {
      if (!foundClock)return true;

      DateTime now = rtc.now();
      
      return
        now.hour() * 60 + now.minute() >= startHour * 60 + startMinute &&
        now.hour() * 60 + now.minute() < endHour * 60 + endMinute;
    }

    bool doRestart(){
      if (!foundClock)return false;

      DateTime now = rtc.now();
      
      uint8_t currentMinute = now.hour() * 60 + now.minute();
      uint8_t restartMinute = restartHour * 60 + restartMinute;
      bool myResult = currentMinute == restartMinute && currentMinute < lastMinute;
      lastMinute = currentMinute;
      return myResult;
    }
};



#endif
