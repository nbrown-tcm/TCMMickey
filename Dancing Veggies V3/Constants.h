/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Constants_h
#define Constants_h

#include "Arduino.h"

#define pingPin 8
#define echoPin 9

#define buttonPin 13
#define motorOnePin 2
#define motorTwoPin 10
#define motorThreePin 5

// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS      7      // VS1053 chip select pin (output)
#define SHIELD_DCS     6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin


#endif