#ifndef ChronosSeven_h
#define ChronosSeven_h

#if ARDUINO >= 100     // Arduino 1.0 and 0023 compatible!
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class ChronosSeven
{
private:
    int _latchPin, _clockPin, _dataPin;
    int time[4];
    byte segmentStates[4];
    int pinState;

    // "Static" variables
    int registerCount;
    byte empty;
    byte full;
    byte numeral[10];

    void setHoursTens(int h);
    void setHoursOnes(int h);
    void setMinutesTens(int m);
    void setMinutesOnes(int m);
    int getDigit(int n, int d);
    void shiftOut();

public:
    ChronosSeven();
    void setPins(int latchPin, int clockPin, int dataPin);
    void setTime(int h, int m);
    void setHours(int h);
    void setMinutes(int m);
    void clear();
    void blast();
    void display();
};


#endif
