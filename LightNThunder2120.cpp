/*This code is based upon:
  RANDOM MAKING ENCOUNTERS - DELAND CRAVEN
  https://randommakingencounters.com/index.php/2020/09/27/lightning-and-thunder-effect-arduino-dfplayer-mini-neopixels/
  Adafruit
  https://learn.adafruit.com/adafruit-neopixel-uberguide
  Indrek Luuk - Circuit Journal
  circuitjournal.com/how-to-use-the-dfplayer-mini-mp3-module-with-an-arduino
  One Guy, One Blog
  oneguyoneblog.com/2017/11/01/lightning-thunder-arduino-halloween-diy/
*/


#include <Arduino.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <Adafruit_DotStar.h>
#include <SPI.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define DATAPIN 11
#define CLOCKPIN 13
#define NUMPIXELS 72
// set number of pixels in strip
//int NUMPIXELS = 72;
// set pin to control DotStars
int DotStar = 11;

// set initial brightness 0-255
int brightness = 255;

// create led object
Adafruit_DotStar ledStrip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);

// assign pins to TX and RX for player
static const uint8_t PIN_MP3_TX = 2;
static const uint8_t PIN_MP3_RX = 3;
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

// create the player object
DFRobotDFPlayerMini myPlayer;

void setup()
{
  // initialize DotStars
  ledStrip.begin();                   // strip.begin();
  ledStrip.setBrightness(brightness); //strip.setBrightness(brightness);
  ledStrip.show();                    //strip.show();

  // initialize serial port for output
  Serial.begin(9600);
  // initialize player serial port
  softwareSerial.begin(9600);

  // connect to player - print result
  if (myPlayer.begin(softwareSerial))
  {
    Serial.println("Connection successful.");
    // set initial volume 0-30
    myPlayer.volume(30);
  }
  else
  {
    Serial.println("Connection failed.");
  }
}

void loop()
{
  // volume defines both the led brightness and delay after flash
  int volMin = 15;
  int volMax = 31;
  int randomVol = random(volMin, volMax);

  // upper value should be one more than total tracks
  int randomTrack = random(1, 9);

  // lightning variables
  // use rgbw DotStar adjust the following values to tweak lightning base color
  int r = random(40, 80);
  int g = random(10, 25);
  int b = random(0, 10);
  
  // return 32 bit color
  //uint32_t color = ledStrip.Color(r, g, b, 50);
  //uint32_t randColor();
  uint32_t Color();
  //uint32_t* Color;
  {
    uint32_t Color = 0x000000; //GRB

    uint8_t red = random(0, 255);
    uint8_t blue = random(0, 255);
    uint8_t green = random(0, 255);

    Color = Color | (green << 16);
    Color = Color | (red << 8);
    Color = Color | blue;

    //return color;
  }

  // number of flashes
  int flashCount = random(5, 15);
  // flash white brightness range - 0-255
  int flashBrightnessMin = 10;
  int flashBrightnessMax = 255;
  // flash duration range - ms
  int flashDurationMin = 5;
  int flashDurationMax = 75;
  // flash off range - ms
  int flashOffsetMin = 0;
  int flashOffsetMax = 75;
  // time to next flash range - ms
  int nextFlashDelayMin = 1;
  int nextFlashDelayMax = 50;
  // map white value to volume - louder is brighter
  int flashBrightness = map(randomVol, volMin, volMax, flashBrightnessMin, flashBrightnessMax);

  // map flash to thunder delay - invert mapping
  int thunderDelay = map(randomVol, volMin, volMax, 1000, 250);

  // randomize pause between strikes
  // longests track length - ms
  int longestTrack = 18000;
  // intensity - closer to longestTrack is more intense
  int stormIntensity = 30000;
  long strikeDelay = random(longestTrack, stormIntensity);

  // debug serial print
  Serial.println("FLASH");
  Serial.print("Track: ");
  Serial.println(randomTrack);
  Serial.print("Volume: ");
  Serial.println(randomVol);
  Serial.print("Brightness: ");
  Serial.println(flashBrightness);
  Serial.print("Thunder delay: ");
  Serial.println(thunderDelay);
  Serial.print("Strike delay: ");
  Serial.println(strikeDelay);
  Serial.print("-");

  for (int flash = 0; flash <= flashCount; flash += 1)
  {
    // add variety to color
    int colorV = random(0, 50);
    if (colorV < 0)
      colorV = 0;
    // flash segments of DotStar strip
    Color = ledStrip.Color(r + colorV, g + colorV, b + colorV, flashBrightness);
    ledStrip.fill(colorV, 0, 4);
    ledStrip.show();
    delay(random(flashOffsetMin, flashOffsetMax));
    ledStrip.fill(colorV, 8, 4);
    ledStrip.show();
    delay(random(flashOffsetMin, flashOffsetMax));
    ledStrip.fill(colorV, 4, 4);
    ledStrip.show();
    delay(random(flashOffsetMin, flashOffsetMax));
    ledStrip.fill(colorV, 9, 14);
    ledStrip.show();
    delay(random(flashDurationMin, flashDurationMax));
    ledStrip.clear();
    ledStrip.show();
    delay(random(nextFlashDelayMin, nextFlashDelayMax));
  }
  // pause between flash and thunder
  delay(thunderDelay);

  // trigger audio - randomize volume and track
  myPlayer.volume(randomVol);
  myPlayer.play(randomTrack);

  delay(strikeDelay);
}
