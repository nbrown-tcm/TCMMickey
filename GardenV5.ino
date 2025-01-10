#include <Adafruit_NeoPixel.h>
//#include <Ramp.h>                           


// Pattern types supported:
enum  pattern { NONE_, TRICKLE, SPLOOSH, SCANNER, SPLASH };
// Patern directions supported:
enum  direction { FORWARD_, REVERSE_ };

// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel
{
    public:

    // Member Variables:  
    pattern  ActivePattern;  // which pattern is running
    direction Direction;     // direction to run the pattern

    bool lightShouldBeMoving = true;
    
    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position
    
    uint32_t Color1, Color2;  // What colors are in use
    uint16_t TotalSteps;  // total number of steps in the pattern
    uint16_t Index;  // current step within the pattern

    float pudSpeed = 1.7;
    
    void (*OnComplete)();  // Callback on completion of pattern
    
    // Constructor - calls base-class constructor to initialize strip
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
    :Adafruit_NeoPixel(pixels, pin, type)
    {
        OnComplete = callback;
          //myRamp.go(10 , pixels * Interval, LINEAR, LOOPBACKWARD);    

    }
    
    // Update the pattern
    void Update()
    {
        if((millis() - lastUpdate) > Interval) // time to update
        {
            lastUpdate = millis();
            switch(ActivePattern)
            {
                case NONE_:
                    NoneUpdate();
                    break;
                case TRICKLE:
                    TrickleUpdate();
                    break;
                case SPLOOSH:
                    SplooshUpdate();
                    break;
                case SCANNER:
                    ScannerUpdate();
                    break;
                case SPLASH:
                    SplashUpdate();
                    break;
                default:
                    break;
            }
        }
    }
  
    // Increment the Index and reset at the end
    void Increment()
    {
        if (Direction == FORWARD_)
        {
           Index++;
           if (Index >= TotalSteps)
            {
                Index = 0;
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
            }
        }
        else // Direction == REVERSE
        {
            --Index;
            if (Index <= 0)
            {
                Index = TotalSteps-1;
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
            }
        }
    }
    
    // Reverse pattern direction
    void Reverse()
    {
        if (Direction == FORWARD_)
        {
            Direction = REVERSE_;
            Index = TotalSteps-1;
        }
        else
        {
            Direction = FORWARD_;
            Index = 0;
        }
    }

    void None()
    {

        ActivePattern = NONE_;
        Interval = 0;
        TotalSteps = 0;
        Index = 0;

    }

    void NoneUpdate()
    {

        Color(0, 0, 0);
        clear();
        show();
      
    }

    void Sploosh(uint8_t interval, direction dir = FORWARD_)
    {

        ActivePattern = SPLOOSH;
        Interval = interval;
        TotalSteps = 255;
        Index = 0;
        Direction = dir;

    }

    void SplooshUpdate()
    {
    }
    
    void Trickle(uint32_t color1, uint8_t interval, direction dir = REVERSE_)
    {

        ActivePattern = TRICKLE;
        Interval = interval;
        //TotalSteps = (numPixels() - 1) * 2;
        TotalSteps = 38;
        //Direction = dir;
        Color1 = color1;
        Index = 1;

    }

    void TrickleUpdate()
    {


                 Serial.println("Trickling");


        for (int i = 0; i < 38; i++)
        {
             if (i == TotalSteps - Index) // Scan Pixel to the left
             {
                 setPixelColor(i, Color1);

             }
             //else if (i == TotalSteps) ActivePattern = NONE_;

             else // Fading tail
             {
                  setPixelColor(i, DimColor(getPixelColor(i)));
             }
        }
        show();
        Increment();




    }

  void Splash(int interval)
  {

        ActivePattern = SPLASH;
        Interval = interval;
        //TotalSteps = (numPixels() - 1) * 2;
        TotalSteps = 38;
        //Direction = dir;
        //Color1 = color1;
        Index = 1;

  }


  void SplashUpdate()
  {


    //used golden ratio to calculate this

    // unsigned long currentMillis = millis();

    //  if(currentMillis - lastUpdate >= Interval)
    //  {
    RingLightUp(0, 8, pudSpeed * 50);
    RingLightUp(8, 16, pudSpeed * 80.9);
    RingLightUp(24, 24, pudSpeed * 130.8962);
    RingLightUp(48, 35, pudSpeed * 211.7900516);
    RingLightUp(83, 45, pudSpeed * 342.676303489);
    //lastUpdate = currentMillis;

    Serial.println("SPlash!!");

    ActivePattern = NONE_;

    //}

    clear();

  }

    // Initialize for a SCANNNER
    void Scanner(uint32_t color1, uint8_t interval, direction dir = REVERSE_)
    {
        ActivePattern = SCANNER;
        Interval = interval;
        TotalSteps = (numPixels() - 1) * 2;
        Color1 = color1;
        Index = 30;
    }

    // Update the Scanner Pattern
    void ScannerUpdate()
    { 
      


        if((millis() - lastUpdate) > Interval) // time to update
        {

        for (int i=50;  i--> 0;)
        {
            if (i == TotalSteps - Index) // Scan Pixel to the left
            {
                 setPixelColor(i, Color1);
                  Serial.println("Scanning the other way now");
                  Serial.println(i);

            }
            else // Fading tail
            {
                 setPixelColor(i, DimColor(getPixelColor(i)));
            }
        
        }
        show();
        Increment();
        }
    }

    void RingLightUp(int start, int length, int del)
    {

    fill((25, 25, 200), start, length);
    show();
    delay(del);
    clear();
    Serial.println("Lighting...");

  }
  
   
    // Calculate 50% dimmed version of a color (used by ScannerUpdate)
    uint32_t DimColor(uint32_t color)
    {
        // Shift R, G and B components one bit to the right
        uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
        return dimColor;
    }

    // Set all pixels to a color (synchronously)
    void ColorSet(uint32_t color)
    {
        for (int i = 0; i < numPixels(); i++)
        {
            setPixelColor(i, color);
        }
        show();
    }

    // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color)
    {
        return (color >> 16) & 0xFF;
    }

    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color)
    {
        return (color >> 8) & 0xFF;
    }

    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color)
    {
        return color & 0xFF;
    }
    
};

#include <MegaMotoHB.h>

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

MegaMotoHB motor(11, 10);


// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

int lastUpdate = 0;

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

void PuddleComplete();
void Ring1Complete();
void Ring2Complete();
void Ring3Complete();

NeoPatterns Ring1(40, 12, NEO_GRB + NEO_KHZ800, &Ring1Complete);
NeoPatterns Ring2(40, 8, NEO_GRB + NEO_KHZ800, &Ring2Complete);
NeoPatterns Ring3(40, 9, NEO_GRB + NEO_KHZ800, &Ring3Complete);
NeoPatterns PuddleRing(128, 2, NEO_GRB + NEO_KHZ800, &PuddleComplete);

  
void setup() 
{
  Serial.begin(115200);
  Serial.println("Adafruit VS1053 Simple Test");

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
  PuddleRing.ActivePattern = NONE_;
  //PuddleRing.Splash(5);
  //Ring1.Trickle(Ring1.Color(25,25,200), 1);
  //Ring2.Trickle(Ring2.Color(25,25,200), 1);
  //Ring3.Trickle(Ring3.Color(25,25,200), 1);

  Ring1.ActivePattern = NONE_;
  Ring2.ActivePattern = NONE_;
  Ring3.ActivePattern = NONE_;


  //Ring1.Scanner(Ring1.Color(255,0,0), 55);

  PuddleRing.begin();
  Ring1.begin();
  Ring2.begin();
  Ring3.begin();

}

void loop() 
{

    unsigned long currentMillis = millis();


    int val = digitalRead(5);


    if (val == 0) //Beam is broken
    {
      //Serial.println("Water the damn plant already");
    }
    else
    {


      //waits 3 seconds before raising motor
      if(currentMillis - lastUpdate >= 1000)
      {

        unsigned long motorMillis = millis();
        unsigned long motorUpdate;

      motor.Fwd(255);

      // Wait for 1 second
      delay(2000);

      // Move backward at half speed
      motor.Rev(255);

      // Wait for 1 second
      delay(2000);

        // if(motorMillis - motorUpdate >= 2000)
        // {
        //   motor.Rev(255);
        // }
        // else
        // {
        //   motor.Fwd(178);
        // }

        //maybe add a function to increase this motor speed the longer it is watered

        // Wait for 1 second
        //delay(2000);

        // Move backward at half speed

        // Wait for 1 second
        //delay(2000);


        lastUpdate = currentMillis;
      }





      Serial.println(F("Playing Water Flow Sound"));
      //musicPlayer.startPlayingFile("/_bradley.mp3");
      //musicPlayer.startPlayingFile("/BlubBlub.mp3");

      PuddleRing.ActivePattern = SPLASH;

      PuddleRing.Splash(5); 
      Ring1.ActivePattern = TRICKLE;
      Ring1.Trickle(Ring1.Color(25,25,200), 50);
      Ring2.ActivePattern = TRICKLE;
      Ring2.Trickle(Ring2.Color(25,25,200), 50);
      Ring3.ActivePattern = TRICKLE;
      Ring3.Trickle(Ring3.Color(25,25,200), 50);  
      

      

  }

  if (Serial.available()) 
  {
    char c = Serial.read();
    
    if (c == 'w') 
    {

      Serial.println(F("Playing Water Flow Sound"));
      //musicPlayer.startPlayingFile("/_bradley.mp3");
      musicPlayer.startPlayingFile("/BlubBlub.mp3");

      PuddleRing.ActivePattern = SPLASH;

      PuddleRing.Splash(5); 
      Ring1.ActivePattern = TRICKLE;
      Ring1.Trickle(Ring1.Color(25,25,200), 50);
      Ring2.ActivePattern = TRICKLE;
      Ring2.Trickle(Ring2.Color(25,25,200), 50);
      Ring3.ActivePattern = TRICKLE;
      Ring3.Trickle(Ring3.Color(25,25,200), 50);  

      motor.Fwd(255);

      // Wait for 1 second
      delay(2000);

      // Move backward at half speed
      motor.Rev(255);

      // Wait for 1 second
      delay(2000);

      Serial.println("Sploosh");

    }

    if (c == 'm') 
    {

      Serial.println("going up");

      motor.Fwd(255);

      // Wait for 1 second
      delay(5000);

      Serial.println("going down");


      // Move backward at half speed
      motor.Rev(255);

      // Wait for 1 second
      delay(5000);
      //Serial.println("Moving motor");


    }

  }

        PuddleRing.Update();
        Ring1.Update();
        Ring2.Update();
        Ring3.Update();


}


/// File listing helper
void printDirectory(File dir, int numTabs) 
{
   while(true) 
   {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

void PuddleComplete()
{
    //PuddleRing.clear();
    //PuddleRing.None();

    //PuddleRing.lightShouldBeMoving = false;

}

void Ring1Complete()
{

  //Ring1.None();
  //Ring1.clear();
  //Ring1.lightShouldBeMoving = false;
  Serial.println("Flow done");
  Ring1.ActivePattern = NONE_;


}

void Ring2Complete()
{

  //Ring2.None();
  //Ring2.clear();
  //Ring1.lightShouldBeMoving = false;
  Serial.println("Flow done");
  Ring2.ActivePattern = NONE_;


}

void Ring3Complete()
{

  //Ring3.None();
  //Ring3.clear();
  //Ring1.lightShouldBeMoving = false;
  Serial.println("Flow done");
  Ring3.ActivePattern = NONE_;


}