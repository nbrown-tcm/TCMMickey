//invokes the MegaMoto Library
#include <MegaMotoHB.h>
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include "Variables.h"

//Objects
//----------------------------------------

class CakePusher
{
	// Class Member Variables
	// These are initialized at startup
	int ledPin;      // the number of the LED pin
	long OnTime;     // milliseconds of on-time
	long OffTime;    // milliseconds of off-time

	// These maintain the current state
	int ledState;             		// ledState used to set the LED
	unsigned long previousMillis;  	// will store last time LED was updated

  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Flasher(int pin, long on, long off)
  {
	ledPin = pin;
	pinMode(ledPin, OUTPUT);     
	  
	OnTime = on;
	OffTime = off;
	
	ledState = LOW; 
	previousMillis = 0;
  }

  void Begin()
  {

    

  }

  void Update()
  {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
     
    if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
    	ledState = LOW;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      digitalWrite(ledPin, ledState);  // Update the actual LED
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
      ledState = HIGH;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      digitalWrite(ledPin, ledState);	  // Update the actual LED
    }
  }
};



//Variables
//--------------------------------------------------------

//Set this for the length of the birthday song audio that plays
int seconds = 5;

//this is a boolean flag that checks if the cake is ready to reset after completing the birthday animation. This makes sure the animation doesn't loop, instead resetting when one of the candles is removed. 
bool isCakeReady = true;

//sets a variable for the motor and the timers it will use
MegaMotoHB motor(9, 10);


Adafruit_VS1053_FilePlayer musicPlayer = 
 
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

  

//------------------------------------------------

void setup() {
	Serial.begin(9600);

  Serial.println("Mickey's Cake is online...");

    if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));

  // Enable internal pull-up for the chain of reed switches
	pinMode(REED_CHAIN_PIN, INPUT_PULLUP);	

  // Enable internal pull-up for the center reed switch
	pinMode(REED_MIDDLE_PIN, INPUT_PULLUP);	

  //Sets the pin for the LED inside the cake as well as defining it as an output
  pinMode(LED_PIN, OUTPUT);


   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

//Serial.print("huh");

  // list files
  // printDirectory(SD.open("/"), 0);
  
  // // Set volume for left, right channels. lower numbers == louder volume!
   musicPlayer.setVolume(20,20);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
   musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
  // // Play one file, don't return until complete
  // Serial.println(F("Playing track 001"));
  // musicPlayer.playFullFile("/track001.mp3");
  // // Play another file in the background, REQUIRES interrupts!
  // Serial.println(F("Playing track 002"));
  // musicPlayer.startPlayingFile("/track002.mp3");

  //plays the startup animation
  StartupAnimation();

}

//--------------------------------------------------------

void loop() {

  // Read the state of the switch
	int proximityChain = digitalRead(REED_CHAIN_PIN); 

  // Read the state of the switch
  int proximityMiddle = digitalRead(REED_MIDDLE_PIN); 

  // // File is playing in the background
  // if (musicPlayer.stopped()) {
  //   Serial.println("Done playing music");
  //   while (1) {
  //     delay(10);  // we're done! do nothing...
  //   }
  // }
  // if (Serial.available()) {
  //   char c = Serial.read();
    
  //   // if we get an 's' on the serial console, stop!
  //   if (c == 's') {
  //     musicPlayer.stopPlaying();
  //   }
    
  //   // if we get an 'p' on the serial console, pause/unpause!
  //   if (c == 'p') {
  //     if (! musicPlayer.paused()) {
  //       Serial.println("Paused");
  //       musicPlayer.pausePlaying(true);
  //     } else { 
  //       Serial.println("Resumed");
  //       musicPlayer.pausePlaying(false);
  //     }
  //   }
  

	
	// If the both of the switch pins read low(candles all placed) and cake is reset, then raise the cake
	if (proximityMiddle == LOW && proximityChain == LOW && isCakeReady == true) {

    Serial.println("Conditions met; Raise the cake");

    // turn the LED strip on (HIGH is the voltage level)
    digitalWrite(LED_PIN, HIGH);   

    //move the motor forward
    motor.Fwd(255);

    musicPlayer.playFullFile("/Birfdaye.mp3");


    // Set this time based on how long the song is supposed to play
    delay(seconds * 1000);

    //
    isCakeReady = false;


	}
  //this lowers the cake after it has been raised and does not raise it again until a candle has been removed and replaced
  //candles are on the cake but cake is not ready
	else if (proximityMiddle == LOW && proximityChain == LOW && isCakeReady == false){
		Serial.println("Conditions met and ended; Lower the Cake");

    //Withdraws the cake motor
    motor.Rev(255);
    
		digitalWrite(LED_PIN, LOW);		// Turn the LED off


    
	}
  //This lowers the cake when cake the condition for raising has not been met (ie candles not placed)
  //candles are not on the cake and the cake is not ready to lift
  else if (proximityMiddle == HIGH || proximityChain == HIGH && isCakeReady == false){

		Serial.println("Conditions not met lower the Cake");

    //Withdraws the cake motor
    motor.Rev(255);
    
		digitalWrite(LED_PIN, LOW);		// Turn the LED off

    isCakeReady = true;


  }
}

//This animation tests the basic output functions when started up so that the operator knows they're working
void StartupAnimation(){

  Serial.println("Testing Animation...");

//raises the motor
  motor.Fwd(255);
  delay(2500);

//flashes the led sign
  digitalWrite(LED_PIN, HIGH);		// Turn the LED off
  delay(250);

	digitalWrite(LED_PIN, LOW);		// Turn the LED off
  delay(250);

	digitalWrite(LED_PIN, HIGH);		// Turn the LED off
  delay(250);

	digitalWrite(LED_PIN, LOW);		// Turn the LED off
  delay(1000);

  //this plays a test audio track
  musicPlayer.playFullFile("/Birfdaye.mp3");
  Serial.print("Testing Audio");


//lowers the motor
  motor.Rev(255);
  delay(2500);

}

// /// File listing helper
// void printDirectory(File dir, int numTabs) {
//    while(true) {
     
//      File entry =  dir.openNextFile();
//      if (! entry) {
//        // no more files
//        //Serial.println("**nomorefiles**");
//        break;
//      }
//      for (uint8_t i=0; i<numTabs; i++) {
//        Serial.print('\t');
//      }
//      Serial.print(entry.name());
//      if (entry.isDirectory()) {
//        Serial.println("/");
//        printDirectory(entry, numTabs+1);
//      } else {
//        // files have sizes, directories do not
//        Serial.print("\t\t");
//        Serial.println(entry.size(), DEC);
//      }
//      entry.close();
//    }
// }

