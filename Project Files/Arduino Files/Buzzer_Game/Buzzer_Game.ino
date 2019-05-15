
#include "ledStrip.h"
#include <TimedAction.h>
#include <IRremote.h> // For IR receiver input

enum IR_SIGNAL {
  Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
  CHplus, CH, CHminus, PREV, NEXT, VOLdown, VOLup, EQ, HundredPlus, TwoHundredPlus, PLAYPAUSE
};

const int MAX_LEVELS = 8;

// SET THE PIN CONSTANTS
const int LED_STRIP = 4;
const int RECV_PIN = 3;
const int BUZZER = 2;
const int TEMP_SENSOR = 0;

unsigned int red, green, blue;
int LEVEL = 1; // Increments as player progresses
int SCORE = 0; // +1 for each correct guess
double TEMP = 0.00; // holds current temperature
String USER_ID = "";
bool fanOn = false;
bool existingPlayer = false;
bool readyToUpdatePlayerRecord = false;

IR_SIGNAL USER_INPUT;
IRrecv irrecv(RECV_PIN);
decode_results results; // Holds raw input from sensor
ledStrip leds = ledStrip(8, LED_STRIP);


/** TIMED ACTION **/

// Functions for a timed action must be defined BEFORE the timed action variable
void transmitData()
{

  getTemp(); // Make sure we're transmitting latest data
  TEMP = 20.25;
  Serial.print(String(TEMP).length());
  Serial.print(TEMP);
  
  if (USER_ID.length() == 4) // If we have a current player, update their score
  {
    Serial.print(1);
    Serial.print(USER_ID);
    Serial.print(LEVEL);
  }
  else
  {
    Serial.print(0); // If we dont have a player let the pi know
  }

  Serial.println(); // EOL or '\n' indicates to pi to stop reading
}

// Timed actions are used here so that whilst we wait for user input, 
// we still update the Cloud with our sensor data
TimedAction updateThread = TimedAction(3000, readData);
/** updateThread.check(); Needs to be in EVERY LOOP  **/


void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver for the IR sensor

  /*** Set DIGITAL pins to output information ***/
  pinMode(LED_STRIP, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}


/*  MAIN FUNCTION  */
void loop()
{
  leds.resetLEDS();
  getTemp();

  updateThread.check(); // Check if its time to fire Thread function

  USER_INPUT = waitForIRSignal(); // NO Code should be executed without a form of user input
  void messageRecieved(); // Flash led to indicate valid IR signal received

  switch (USER_INPUT)
  {
    case PLAYPAUSE: // Player presses play/pause to start game
      // Serial.println('GAME INITIATED');
      setUserID();
      startGame();
      break;
  }

  irrecv.resume(); // Receive the next value

  delay(3000);

}

void readData()
{
  String sBuffer, data;
  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill
    if (Serial.available() > 0) {
      char c = Serial.read();  //gets one byte from serial buffer
      sBuffer += c; //makes the string readString
    }
  }

  if (sBuffer.length() > 0) {
    // Serial.println(sBuffer); //see what was received

    // expect a string like 07002100 containing the two servo positions
    data = sBuffer.substring(0, 1);
    if (data == "1")
    {
      fanOn = true;
      // Serial.println("Fan ON");
    }
    else
    {
      fanOn = false;
      // Serial.println("Fan OFF");
    }

    /*   data = sBuffer.substring(1, 2);
       if (data == "1" && readyToUpdatePlayerRecord) // This player already exists & we need to set their level
       {
         USER_ID = sBuffer.substring(2, 6).toInt();
         // Serial.println("Existing PLayer: " + USER_ID);
         LEVEL = sBuffer.substring(6, 7).toInt();
       }

    */

  }
  sBuffer = "";
  transmitData();
}
// count = no. of buzzes
// msDelay = length of buzz &
void buzz(int count)
{
  int msDelay = 1000 / LEVEL;
  int msLength = 1000 / (SCORE + LEVEL);
  
  for (int i = 0; i < count; i++)
  {
    updateThread.check(); // Check if its time to fire Thread function
    digitalWrite(BUZZER, HIGH);
    delay(msLength);
    digitalWrite(BUZZER, LOW);
    delay(msDelay);
  }
}


// Loop until signal received, return corresponding button pressed as string
IR_SIGNAL waitForIRSignal()
{
  while (true)
  {
    irrecv.enableIRIn(); // restart receiver
    updateThread.check(); // Check if its time to fire Thread function

    while (!irrecv.decode(&results))
    {
      updateThread.check(); // Check if its time to fire Thread function
      irrecv.resume(); // Receive the next value
      delay(200);
      if (irrecv.decode(&results))
        break;
    }

    switch (results.value)
    {
      case 0xFF6897: return Zero;
      case 0xFF30CF: return One;
      case 0xFF18E7: return Two;
      case 0xFF7A85: return Three;
      case 0xFF10EF: return Four;
      case 0xFF38C7: return Five;
      case 0xFF5AA5: return Six;
      case 0xFF42BD: return Seven;
      case 0xFF4AB5: return Eight;
      case 0xFF52AD: return Nine;
      case 0xFFA25D: return CHplus;
      case 0xFF269D: return CH;
      case 0xFFE21D: return CHminus;
      case 0xFF22DD: return PREV;
      case 0xFF02FD: return NEXT;
      case 0xFFE01F: return VOLdown;
      case 0xFFA857: return VOLup;
      case 0xFF906F: return EQ;
      case 0xFF9867: return HundredPlus;
      case 0xFFB04F: return TwoHundredPlus;
      case 0xFFC23D: return PLAYPAUSE;
    }
    delay(100);
  }
}

void getTemp()
{
  TEMP = analogRead(A0);
  TEMP = log(((10240000 / TEMP) - 10000));
  TEMP = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * TEMP * TEMP )) * TEMP);
  TEMP = TEMP - 273.15; // Convert Kelvin to Celcius
}


void startGame()
{
  leds.ledCountDown(); // Signal to user we have started

  for (int i = 0; i < MAX_LEVELS; i ++)
  {
    if (!playLevel())
    {
      for (int j = 0; j < 3; j++)
      {
        transmitData();
        delay(2000);
      }
      USER_ID = "";
      LEVEL = 1;
      break;
    }
    else
      LEVEL++; // We advance to the next level
  }
}


bool playLevel()
{
  bool GAME_OVER = false;
  // Level 1 = 1 full second, level 2 = 0.5 seconds etc...


  for (int i = 0; i < 3 && !GAME_OVER; i++)
  {
    leds.showLevel(LEVEL);
    updateThread.check(); // Check if its time to fire Thread function

    int noOfBuzzes = random(1, 9); // Give a random no. of tones to guess
    buzz(noOfBuzzes); // Play the tones for the player

    // Serial.println("Buzzes: " + String(noOfBuzzes));
    IR_SIGNAL guess = waitForIRSignal();

    // Serial.println(guess);

    if (guess == noOfBuzzes) // Get the user input == difficulty)
    {
      leds.messageRecieved();
      SCORE++;
    }
    else
    {
      GAME_OVER = true; // Tell startGame() the player failed
      leds.incorrect();
      return false;
    }
  }
  return true; // Tell startGame() they completed it successfully
}


void setUserID()
{
  // Serial.println("Waiting for user Input:");
  USER_ID = ""; // reset any residual data

  while (USER_ID.length() != 4)
  {
    IR_SIGNAL nextLetter = PLAYPAUSE;
    leds.setName(4 - USER_ID.length()); // show user how many chars remain

    while (nextLetter > 9) // only accept single digit numbers
    {
      updateThread.check(); // send data every 3 seconds
      nextLetter =  waitForIRSignal();
      Serial.println("waitin for IR");
    }
    leds.messageRecieved();

    USER_ID.concat(nextLetter); // prevents moving underlying char* pointer
    // Serial.println(USER_ID);
  }

  checkIfPlayerExists();
}

void checkIfPlayerExists()
{
  // Allow readData to write to USER_ID var to update it
  readyToUpdatePlayerRecord = true;

  for (int i = 0; i < 5; i ++)
  {
    readData(); // read 5 times @1 sec intervals
    green = 150; blue = 51; red = 255;
    leds.updateLed(7,  red, green, blue);
    delay(200);
    leds.resetLEDS();
    delay(800);
  }
  readyToUpdatePlayerRecord = false; // reset it so a read operation does not interupt game
}
