/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Variable_h
#define Variable_h

#include "Arduino.h"

//This Variable refers to the daisy chain of reeds around the center of the cake
//They all act as one switch due to being daisy chained
//This variable refers to the reed switch in the middle of the cake
#define REED_MIDDLE_PIN 8	// Pin connected to reed switch
#define REED_CHAIN_PIN 5	// Pin connected to reed switch

//Sets the pin for the LED strip
#define LED_PIN 2	// LED pin

#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS      7      // VS1053 chip select pin (output)
#define SHIELD_DCS     6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin






#endif