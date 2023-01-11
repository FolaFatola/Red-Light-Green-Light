// PROJECT  :Red Light, Green Light
// PURPOSE  :An IR dependent multiplication game (to answer question).
// AUTHOR   :Fola Fatola
// DATE     :2019 05 29
// COURSE   :ICS3U
// STATUS   :Mostly Working
// REFERENCE:http://darcy.rsgc.on.ca/ACES/TEI3M/1819/Tasks.html


//Libraries used
#include <Keypad.h>
#include <IRremote.h> //for sending IR signals. 
IRsend folaSend;


const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
uint8_t numPress = 0; //count number of presses
uint8_t ones; //ones digit
uint8_t tens; //tens digit
uint8_t key; //key pressed
uint8_t ans; //final answer


//define the cymbols on the buttons of the keypads
uint8_t myKeys[ROWS][COLS] = {
  {1, 2, 3},
  {4, 5, 6},
  {7, 8, 9},
  {42, 48, 35}
};

byte rowPins[ROWS] = {10, 9, 8, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 5, 4}; //connect to the column pinouts of the keypad


uint8_t sentIr [] {0XFF6897,  0xFF30CF, 0xFF18E7, 0xFF7A85, 0xFF10EF, 0xFF38C7, 0xFF5AA5, 0xFF42BD, 0xFF4AB5, 0xFF52AD, 0xFFA857};


//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(myKeys), rowPins, colPins, ROWS, COLS);


void setup() {
  Serial.begin(9600); //begin serial monitor
}


void reset() //reset button. Resets number of press to 0.
{
  if (key == 35)
  {
    numPress = 0;
  }
}

void inputData() {
  key = customKeypad.getKey(); //read the key pressed
  if (key) {
    if (key != NO_KEY) { //NO KEY means no key is pressed. Count the # of presses
      numPress++; //count the number of presses.
    }
    if (numPress == 1) { //first button press = tens digit
      tens = key;
      folaSend.sendNEC(sentIr[tens], 20); //send tens digit IR code
      delay(50);

    }
    if (numPress == 2) { //first button press = ones digit
      ones = key;
      folaSend.sendNEC(sentIr[ones], 20); //send ones digit IR code
      delay(50);
    }
  }
}

void loop() {
  inputData(); //send IR signals.
  reset(); //reset when button known as char 35 is pressed.
}


