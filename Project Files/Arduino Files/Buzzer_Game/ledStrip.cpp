#include "ledStrip.h"

ledStrip::ledStrip(int ledCount, int ledPin)
{
  pixels = Adafruit_NeoPixel(ledCount, ledPin, NEO_RGB + NEO_KHZ800);
}

void ledStrip::countFromBottom(int count, unsigned int r, unsigned int g, unsigned int b)
{
  pixels.clear();

  for (int i = 0; i < count; i++)
  {
    pixels.setPixelColor(i, pixels.Color(g, r, b));
  }

  pixels.show();
}

void ledStrip::messageRecieved()
{
  for (int j = 0; j < 2; j++)
  {
    green = 255; red = 0; blue = 0;
    countFromBottom(8, red, green, blue);
    delay(50);
    resetLEDS();
    delay(100);
  }
}

void ledStrip::setName(int remainingCharacters)
{
  green = 85; red = 25; blue = 255;
  countFromBottom(remainingCharacters, red, green, blue);
}

void ledStrip::incorrect()
{
  for (int j = 0; j < 3; j++)
  {
    green = 0; red = 255; blue = 0;
    countFromBottom(8, red, green, blue);
    delay(150);
    resetLEDS();
    delay(150);
  }
}

void ledStrip::showLevel(int level)
{
  green = 0; red = 230; blue = 191;
  countFromBottom(level, red, green, blue);
}


void ledStrip::showLeds()
{
  pixels.show();
}

void ledStrip::clearLeds()
{
  pixels.clear();
}

void ledStrip::updateLed(int led, unsigned int r, unsigned int g, unsigned int b)
{
  pixels.clear();
  pixels.setPixelColor(led, pixels.Color(g, r, b));
  pixels.show();
}

void ledStrip::trafficLight(int active)
{
  int i = 0;

  for (i; i < 3 && i < active; i ++)
  {
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    
  }
delay(20);

  for (i; i < 5 && i < active; i ++)
  {
    pixels.setPixelColor(i, pixels.Color(170, 255, 0));
  }

  for (i; i < 8 && i < active; i ++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 255, 0));
  }

  pixels.show();
}

void ledStrip::ledCountDown()
{
  for (int i = 8; i != 0; i --)
  {
    trafficLight(i);
    delay(500);
    resetLEDS();
    delay(250);
  }

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
