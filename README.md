# ChronosSeven

The purpose of this Arduino library is to display the current time using four custom 7-segment displays, each controlled by its own shift register. 


## Hardware Setup

Four 8-bit shift registers (e.g. 595 series) should be connected in series, with the first one connected to the Arduino. Each register should have its first 7 pins (e.g. Q0-Q6) connected to seven different LEDs. The first register controlls the first digit of the time (the tens-hour digit) and so forth.

```
ARDUINO -- REGISTER1 -- REGISTER2 -- REGISTER3 -- REGISTER4
               |            |            |            |
Time:          1            2      :     4            5
```

It's important to connect the register pins to the correct LEDs in the display:

```
   -- Q0 --
 |          |
 Q5        Q1
 |          |
   -- Q6 --
 |          |
 Q4        Q2
 |          |
   -- Q3 --

```


## Usage

ChronosSeven does not keep track of the time; that must be done elsewhere. The Arduino must supply ChronosSeven with the hours and minutes as two separate integer values, and told to display them.

First include the library in your Arduino sketch an instantiate an object using the variable `chronos`:

```Arduino
#include <ChronosSeven.h>

ChronosSeven chronos;
```

Inside of the `setup` function, ChronosSeven must be supplied with which Arduino output pins are connected to the first shift register:

```Arduino
void setup() {
    int latchPin = 8;  // Connected to ST_CP of 74HC595
    int clockPin = 12; // Connected to SH_CP of 74HC595
    int dataPin = 11;  // Connected to DS of 74HC595
    chronos.setPins(latchPin, clockPin, dataPin);
    
    // Other setup code...
}
```

Anywhere within the `loop` function, the time may be set and displayed:

```Arduino
void loop() {
    int hours = 12;
    int minutes = 45;
    chronos.setTime(hours, minutes);
    chronos.display();
}
```


## API

Unless otherwise noted, most functions are used to manipulate the bytes inside of the shift registers, and will not affect the display until `display()` is called.

`void setPins(int latchPin, int clockPin, int dataPin)`
Sets the output pins going to the first shift register. Resets the display and sets all register pins to `LOW`.

`void setTime(int h, int m)`
Sets the time that should be displayed. Internally calls `setHours(h)` and `setMinutes(m)`.

`void setHours(int h)`
Sets just the hours that should be displayed, in case you want to control those independently of the minutes.

`void setMinutes(int m)`
Sets just the minutes that should be displayed, in case you want to control those independently of the hours.

`void clear()`
Sets all register pins to `LOW`, effectively turning off the entire display.Opposite of `blast()`.

`void blast()`
Sets all register pins to `HIGH`, effectively turning on the entire display. Opposite of `clear()`.

`void display()`
Triggers the `latchPin` to actually display the data that has been shifted into the registers. This will usually be called after any method that's intended to change the display.
