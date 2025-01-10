// include SPI, MP3, Servo and SD libraries
#include <Servo.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SPI.h>
#include "Constants.h"


int track1, track2, track3;

unsigned int duration, inches;


Adafruit_VS1053_FilePlayer musicPlayer = 
 
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

int motorOneStartDelay = 500;
int motorTwoStartDelay = 600;
int motorThreeStartDelay = 700;
int songDuration = 20000;  // Duration of the Happy Birthday song

int currentStage = 0;
int buttonState = 0;
int lastButtonState = LOW;
bool songPlaying = false;
unsigned long songStartTime = 0;
unsigned long lastStageChangeTime = 0;

void setup() {
  
  Serial.begin(9600);


  track1=0;
  track2=0;
  track3=0;
  pinMode(8,OUTPUT);
  pinMode(9,INPUT);

  pinMode(buttonPin, INPUT);
  pinMode(motorOnePin, OUTPUT);
  pinMode(motorTwoPin, OUTPUT);
  pinMode(motorThreePin, OUTPUT);

  digitalWrite(motorOnePin, LOW);
  digitalWrite(motorTwoPin, LOW);
  digitalWrite(motorThreePin, LOW);




  Serial.println("'Sing to the Vegetables' is online...");

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
  SD.begin(CARDCS);    // initialise the SD card
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(5,5);


  //pinMode(audioPin, OUTPUT);
  //pinMode(lightPin, OUTPUT);

  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

}

//this function pings out and recieves the echo, 
//letting the machine know how far the user is
void ping()  
{
  digitalWrite(pingPin, LOW);        // Ensure pin is low
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);       // Start ranging
  delayMicroseconds(5);              //   with 5 microsecond burst
  digitalWrite(pingPin, LOW);        // End ranging
 
  duration = pulseIn(echoPin, HIGH); // Read echo pulse
  inches = duration / 74 / 2;        // Convert to inches
  Serial.println(inches);            // Display result
  //if ((inches>3) && (inches<7)) { Serial.println("1"); delay(1000); }
  //if ((inches>8) && (inches<12))  { Serial.println("2"); delay(1000);}
  //if ((inches>13) && (inches<17))  { Serial.println("3"); delay(1000) ;}
  
}


void loop() {
  delay(50);

  //calls the ping function to get user distance
  //prints user distance (in inches) to the console
  ping();

  //if the player moves within 40 inches of the device,
  if ((inches>1) && (inches<5)) {
    if (track1==0){
      //play the instructions
      playGardenerInstructions();
    }
  
     else if(track1==1){
      //this repeats the instruction if the button isnt pressed, but theyre still standing in front of the unit
      delay(5000);
      playGardenerInstructions();
    }
  }

  unsigned long currentTime = millis();

  //digitalWrite(0, LOW);

  // Check the button state
  buttonState = digitalRead(buttonPin);

    Serial.println(currentStage);


//this starts the vegetable singing song
//pushes the button and has not before
  if (buttonState == HIGH && lastButtonState == LOW) {
    lastButtonState = buttonState;
    Serial.println("User pushed button!");
    startSong(currentTime);
  }

//this pauses the song if the user stops pushing the button
  if (buttonState == LOW && lastButtonState == HIGH && songPlaying) {
    lastButtonState = buttonState;
    Serial.println("User Stopped pressing button");
    stopSong(currentTime);
  }


  if (songPlaying) {
    if (currentTime - songStartTime >= songDuration) {
      endSong();  // Function to end the song and reset everything
    } 
    else {
      handleMotorStart(currentTime);
      //handleLightingEffects(currentTime);  // Handle light effects based on time
    }
  }
}





void startSong(unsigned long currentTime) {
    Serial.println("Start singing!");
  songPlaying = true;
  songStartTime = currentTime;
  currentStage = 1;
  lastStageChangeTime = currentTime;
  playMusic();  // Function to start playing the Happy Birthday song
}

void stopSong(unsigned long currentTime) {
  songPlaying = false;
  stopMusic();  // Function to stop the music
  resetMotors();  // Function to stop all motors and reset their positions
  //resetLighting();  // Function to stop and reset lighting effects
  currentStage = 0;
}

void handleMotorStart(unsigned long currentTime) {
  switch (currentStage) {
    case 1:
      if (currentTime - lastStageChangeTime >= motorOneStartDelay) {
        digitalWrite(motorOnePin, HIGH);
        lastStageChangeTime = currentTime;
        //playEncouragingFeedback(1);  // Gardener gives feedback for Motor 1
        currentStage++;
      }
      break;

    case 2:
      if (currentTime - lastStageChangeTime >= motorTwoStartDelay) {
        digitalWrite(motorTwoPin, HIGH);
        lastStageChangeTime = currentTime;
        //playEncouragingFeedback(2);  // Gardener gives feedback for Motor 2
        currentStage++;
      }
      break;

    case 3:
      if (currentTime - lastStageChangeTime >= motorThreeStartDelay) {
        digitalWrite(motorThreePin, HIGH);
        //playEncouragingFeedback(3);  // Gardener gives feedback for Motor 3
        currentStage++; // No further stages, motors are all running
      }
      break;
  }
}

// void handleLightingEffects(unsigned long currentTime) {
//   // Implement lighting effects to match the vegetable dancing
// }



void playGardenerInstructions() {
  //play the first audio
  Serial.println("Playing instruction audio");
  musicPlayer.playFullFile("track001.mp3");
  track1=1; track2=0; track3=0;
}

  void playMusic() {
   musicPlayer.playFullFile("/track003.mp3");   
}

void stopMusic() {
  Serial.print("Stop music");
  musicPlayer.stopPlaying();
}

void endSong() {
  stopSong(millis());
  Serial.print("Song Over");
  //playThanks();  // Gardener thanks the users
}

// void playThanks() {
//   // Code to play Gardener's thank you message
// }


// void playEncouragingFeedback(int stage) {
//   // Code to play Gardener's feedback based on which vegetable started dancing
// }



void resetMotors() {
  // Code to reset all motor states
  digitalWrite(motorOnePin, LOW);
  digitalWrite(motorTwoPin, LOW);
  digitalWrite(motorThreePin, LOW);
}

// void resetLighting() {
//   // Code to reset all lighting effects
//   digitalWrite(lightPin, LOW);
// }



