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
  public:
    ledStrip(int ledCount, int ledPin);
    void resetLEDS();
    void ledCountDown(int startingNo);
    void updateLed(int gree, int red, int blue);
      private:
    Adafruit_NeoPixel pixels;
};

#endif
