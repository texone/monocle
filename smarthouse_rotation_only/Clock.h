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
#if defined(ARDUINO_AVR_UNO)
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
#else
      Console.print(now.year(), DEC);
      Console.print('/');
      Console.print(now.month(), DEC);
      Console.print('/');
      Console.print(now.day(), DEC);
      Console.print(" (");
      Console.print(daysOfTheWeek[now.dayOfTheWeek()]);
      Console.print(") ");
      Console.print(now.hour(), DEC);
      Console.print(':');
      Console.print(now.minute(), DEC);
      Console.print(':');
      Console.print(now.second(), DEC);
      Console.println();

      Console.print(" since midnight 1/1/1970 = ");
      Console.print(now.unixtime());
      Console.print("s = ");
      Console.print(now.unixtime() / 86400L);
      Console.println("d");
#endif
    }

    bool isDay() {
      if (!foundClock)return true;

      DateTime now = rtc.now();

      return
        now.hour() >= startHour &&
        now.hour() < endHour &&
        now.minute() >= startMinute &&
        now.minute() < endMinute;
    }
};



#endif
