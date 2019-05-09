
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

int LEVEL = 1; // Increments as player progresses
int SCORE = 0; // +1 for each correct guess
double TEMP = 0.00; // holds current temperature
String USER_ID = "";
bool fanOn = false;
bool existingPlayer = false;

IR_SIGNAL USER_INPUT;
IRrecv irrecv(RECV_PIN);
decode_results results; // Holds raw input from sensor
ledStrip leds = ledStrip(8, LED_STRIP);


/** TIMED ACTION **/

// Functions for a timed action must be defined BEFORE the timed action variable
void transmitData()
{
  String message = ""; // Initialise it
  TEMP = 3225.00;
  message += String(TEMP);
  message = String(message.length())+ TEMP; // String() cast makes sure these values are placed AFTER one another

  USER_ID = "9999";
  
  if (USER_ID != "") // If we have a current player, update their score 
  {
    message += "1";
    message += String(USER_ID);
    message += String(LEVEL);
  }
  else 
    message += "0"; // If we dont have a player let the pi know

    message += "\n"; // EOL char to signal pi
    
  Serial.println(message);
}

// Timed actions are used here so that whilst we wait for user input, we still update the
// Cloud with our sensor data
TimedAction updateThread = TimedAction(1000, readData);
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
  readData();
  delay(3000);
  /*
  leds.ledCountDown(5);
  updateThread.check(); // Check if its time to fire Thread function

  USER_INPUT = waitForIRSignal(); // NO Code should be executed without a form of user input

  Serial.println(USER_INPUT);
  if (irrecv.decode(&results))
  {
    switch (USER_INPUT)
    {
      case PLAYPAUSE: // Player presses play/pause to start game
        Serial.println('GAME INITIATED');
        setUserID();
        startGame();
        break;
    }
  }
  irrecv.resume(); // Receive the next value
  delay(1500);
*/
}

void readData()
{
  String sBuffer, data;
  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      sBuffer += c; //makes the string readString
    }
  }

  if (sBuffer.length() >0) {
      Serial.println(sBuffer); //see what was received
     
      // expect a string like 07002100 containing the two servo positions     
      data = sBuffer.substring(0, 1); 
      if (data == "1")
      {
        fanOn = true;
        Serial.println("Fan ON");
      }
      else 
      {
        fanOn = false;
        Serial.println("Fan OFF");
      }

      data = sBuffer.substring(1, 2); 
      if (data == "1") // This player already exists & we need to set their level
      {
        USER_ID = sBuffer.substring(2,6).toInt();
        Serial.println("Existing PLayer: " + USER_ID);
        LEVEL = sBuffer.substring(6,7).toInt();
      }
    sBuffer="";
  } 
  transmitData();
}

// count = no. of buzzes
// msDelay = length of buzz &
void buzz(int count, int buzzLength, int msDelay)
{
  for (int i = 0; i < count; i++)
  {
    digitalWrite(BUZZER, HIGH);
    delay(buzzLength);
    digitalWrite(BUZZER, LOW);
    delay(msDelay);
  }
}


// Loop until signal received, return corresponding button pressed as string
IR_SIGNAL waitForIRSignal()
{
  irrecv.enableIRIn(); // restart receiver
bool waiting = true;
  
  while (waiting)
  {
    while (!irrecv.decode(&results))
    {
      irrecv.resume(); // Receive the next value
      delay(200);
      if (irrecv.decode(&results))
        break;
    }

    switch (results.value)
    {
      case 0xFF6897:  waiting = false; return Zero;
      case 0xFF30CF:  waiting = false; return One;
      case 0xFF18E7:  waiting = false; return Two;
      case 0xFF7A85:  return Three;
      case 0xFF10EF:  return Four;
      case 0xFF38C7:  return Five;
      case 0xFF5AA5:  return Six;
      case 0xFF42BD:  return Seven;
      case 0xFF4AB5:  return Eight;
      case 0xFF52AD:  return Nine;
      case 0xFFA25D:  return CHplus;
      case 0xFF269D:  return CH;
      case 0xFFE21D:  return CHminus;
      case 0xFF22DD:  return PREV;
      case 0xFF02FD:  return NEXT;
      case 0xFFE01F:  return VOLdown;
      case 0xFFA857:  return VOLup;
      case 0xFF906F:  return EQ;
      case 0xFF9867:  return HundredPlus;
      case 0xFFB04F:  return TwoHundredPlus;
      case 0xFFC23D:  waiting = false; return PLAYPAUSE;
    }
    irrecv.enableIRIn(); // restart receiver
    delay(100);   
  }
}

void getTemp()
{
  TEMP = analogRead(A0);
  TEMP = log(((10240000/TEMP) - 10000));
  TEMP = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * TEMP * TEMP ))* TEMP);
  TEMP = TEMP - 273.15; // Convert Kelvin to Celcius
}


void startGame()
{
  for (int i = 0; i < MAX_LEVELS; i ++)
  {
    if (!playLevel())
      break;
    else
      LEVEL++; // We advance to the next level
  }
}

void setUserID()
{
   Serial.println("Waiting for user Input:");
  for (int i = 0; i <4; i++)
  {
    int nextLetter = waitForIRSignal();
    USER_ID += String(nextLetter);
    Serial.println(USER_ID);
  }
  Serial.println("NEW USER REGISTERED");
  Serial.println(USER_ID);
}

bool playLevel()
{
  bool GAME_OVER = false;
  // Level 1 = 1 full second, level 2 = 0.5 seconds etc...
  int msDelay = 1000 / LEVEL;
  int msLength = 1000 / (SCORE + LEVEL);
  int noOfBuzzes = 0;
  
  for (int i = 0; i < 3 && !GAME_OVER; i++)
  {
    int noOfBuzzes = random(10); // Give a random no. of tones to guess
 //   buzz(noOfBuzzes, msDelay, msLength); // Play the tones for the player

    Serial.println(noOfBuzzes);
    IR_SIGNAL guess = waitForIRSignal();
    Serial.println(guess);
    
    if (guess == noOfBuzzes) // Get the user input == difficulty)
    {
      SCORE++;
    }
    else
    {
      GAME_OVER = true; // Tell playLevel() the player failed
      return false;
    }
  }
  return true; // Tell playLevel() they completed it successfully
}
