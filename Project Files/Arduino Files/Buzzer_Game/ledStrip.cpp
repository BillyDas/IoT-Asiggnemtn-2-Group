#include "ledStrip.h"

ledStrip::ledStrip(int ledCount, int ledPin)
{
  pixels = Adafruit_NeoPixel(ledCount, ledPin, NEO_GRB + NEO_KHZ800);
}



void ledStrip::ledCountDown(int startingNo)
{
  for (int i = startingNo; i > 0; i--)
  {
    resetLEDS();

    delay(200);

    for (int k = 0; k < i; k++)
    {
      pixels.setPixelColor(k, pixels.Color(0, 255, 0));
    }

    pixels.show(); // Send updated colors to hardware

    delay(200);
  }

  for (int i = 0; i < 3; i++)
  {
    resetLEDS();
    delay(100);
    for (int j = 0; j < 8; j++)
    {
      pixels.setPixelColor(j, pixels.Color(0, 255, 0));
    }
    pixels.show(); // Send updated colors to hardware
    delay(100);
  }
  resetLEDS();
}

void ledStrip::resetLEDS()
{
  pixels.clear();
  for (int i = 0; i < 8; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  pixels.show();
}
