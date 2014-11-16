/********************
 bit12LED 
 12LEDs drive library for bitDuino using charlieplexing.

 2014/11/16 @maris_HY
********************/

#ifndef BIT12LED_h
#define BIT12LED_h

#include <inttypes.h>
#include <arduino.h>

void setLEDValue(byte led,byte value);
void TurnOn1LED(byte led);
void TurnOffAllLEDs();
void init12LED(byte pin0,byte pin1,byte pin2,byte pin3);
void start12LED();
void stop12LED();

#endif
