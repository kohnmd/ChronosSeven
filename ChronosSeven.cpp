#include "ChronosSeven.h"

#if ARDUINO >= 100     // Arduino 1.0 and 0023 compatible!
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

ChronosSeven::ChronosSeven()
{
    registerCount = 4;

    numeral[0] = B11111100;
    numeral[1] = B01100000;
    numeral[2] = B11011010;
    numeral[3] = B11110010;
    numeral[4] = B01100110;
    numeral[5] = B10110110;
    numeral[6] = B10111110;
    numeral[7] = B11100000;
    numeral[8] = B11111110;
    numeral[9] = B11100110;

    empty = B00000000;
    full = B11111111;
}

void ChronosSeven::setPins(int latchPin, int clockPin, int dataPin)
{
    _latchPin = latchPin;
    _clockPin = clockPin;
    _dataPin = dataPin;

    pinMode(_latchPin, OUTPUT);
    pinMode(_clockPin, OUTPUT);
    pinMode(_dataPin, OUTPUT);

    clear();
    display();
}

void ChronosSeven::setTime(int h, int m)
{
    setHours(h);
    setMinutes(m);
}

void ChronosSeven::setHours(int h)
{
    setHoursTens(h);
    setHoursOnes(h);
}
void ChronosSeven::setHoursTens(int h)
{
    time[0] = getDigit(h, 0);
    segmentStates[0] = numeral[time[0]];
}

void ChronosSeven::setHoursOnes(int h)
{
    time[1] = getDigit(h, 1);
    segmentStates[1] = numeral[time[1]];
}

void ChronosSeven::setMinutes(int m)
{
    setMinutesTens(m);
    setMinutesOnes(m);
}

void ChronosSeven::setMinutesTens(int m)
{
    time[2] = getDigit(m, 0);
    segmentStates[2] = numeral[time[2]];
}

void ChronosSeven::setMinutesOnes(int m)
{
    time[3] = getDigit(m, 1);
    segmentStates[3] = numeral[time[3]];
}

int ChronosSeven::getDigit(int n, int index) {
    if (n < 10) {
        index -= 1;
    }
    // Convert n to string, get digit at index
    String digits = String(n);
    char digit = digits.charAt(index);
    return String(digit).toInt();
}

void ChronosSeven::clear()
{
    for (int i = 0; i < registerCount; i++) {
        segmentStates[i] = empty;
    }
}

void ChronosSeven::blast()
{
    for (int i = 0; i < registerCount; i++) {
        segmentStates[i] = full;
    }
}

void ChronosSeven::display()
{
    digitalWrite(_latchPin, LOW);
    shiftOut();
    digitalWrite(_latchPin, HIGH);
}

void ChronosSeven::shiftOut()
{
    // Clear everything to prepare for bit shifting
    digitalWrite(_dataPin, LOW);
    digitalWrite(_clockPin, LOW);

    // Count down so that the first bits that are shifted in end up at the last
    // register
    for (int i = (registerCount - 1); i >= 0; i--) {
        for (int j = 0; j < 8; j++)  {
            digitalWrite(_clockPin, LOW);

            // Check the bitmask result at position j.
            if (segmentStates[i] & (1<<j)) {
                pinState = 1;
            } else {
                pinState = 0;
            }

            //Sets the pin to HIGH or LOW depending on pinState
            digitalWrite(_dataPin, pinState);
            //register shifts bits on upstroke of clock pin
            digitalWrite(_clockPin, HIGH);
            //zero the data pin after shift to prevent bleed through
            digitalWrite(_dataPin, LOW);
        }
    }

    //stop shifting
    digitalWrite(_clockPin, LOW);
}
