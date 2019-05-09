/*
 * epilep_C.h - Alibrary for flashing LEDs in various patterns
 * Created by Thomas Wright, 4th May 2019
 * 
 * all LED manipulation is done via the Adafruit_NeoPixel libray
 */

#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <Adafruit_NeoPixel.h> // For controlling the leds

class ledStrip
{
  private:
  unsigned int green;
  unsigned int red;
  unsigned int blue;

  public:
    ledStrip(int ledCount, int ledPin);
    void resetLEDS();
    void ledCountDown();
    void updateLed(int led, unsigned int r, unsigned int g, unsigned int b);
    void showLevel(int level);
    void setName(int remainingCharacters);
    void countFromBottom(int count, unsigned int r, unsigned int g, unsigned int b);
    void messageRecieved();
    void incorrect();
    void trafficLight(int active);
    // getter methods for adafruit library
    void showLeds(); 
    void clearLeds();
      private:
    Adafruit_NeoPixel pixels;
};

#endif
