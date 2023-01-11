// PROJECT  :Red Light, Green Light
// PURPOSE  :An IR dependent multiplication game (to pose question).
// AUTHOR   :Fola Fatola + Chris Darcy
// DATE     :2019 05 29
// COURSE   :ICS3U
// STATUS   :Mostly Working
// REFERENCE:http://darcy.rsgc.on.ca/ACES/TEI3M/1819/Tasks.html

// include the library code:
#include <LiquidCrystal.h> //To use the LCD screen
#include <Stepper.h> //for the stepper motor
#include <Wire.h> //to communicate with I2C devices
#include <TimerOne.h> //for use of millis();
#include <IRremote.h> //for receiving IR signals
#define RECV_PIN 5 //receiver pin 1
#define RECV_PIN2 6 //receiver pin 2

//difficulty level.
unsigned long timer;
unsigned long difficulty = 5000;

//reads IR data through these pins and decodes results
IRrecv irrecv(RECV_PIN);
IRrecv irrecv2(RECV_PIN2);
decode_results results; //results data structure

uint8_t i = 0; //Number of IR signals received
uint8_t j = 0; //LCD factor character
uint8_t a; //one of the factors
uint8_t b; //second factor pair
uint8_t c; //stored product
uint8_t correctAnswer = a * b; //product of factors
uint8_t ones; //tens digit value printed to the LCD screen
uint8_t tens; //ones digit value printed to the LCD screen
uint8_t finalAnswer; //your answer

//Motor setup
uint8_t motorSpeed = 100;
uint16_t stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, A1, A2, A3, A4); //motor pins

//Proximity Sensors.
uint8_t transmitterVal;
uint8_t receiverVal;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600); //start serial monitor
  irrecv.enableIRIn(); //Start the Detector
  pinMode(2, OUTPUT);  //green light
  pinMode(3, OUTPUT);  //red light
}

void receive() {
  if (irrecv.decode(&results) || irrecv2.decode(&results)) {demodulate & record
   i++; //Number of IR codes sent.
    switch (results.value) { //received IR code converted to specific character.
      case 1118642636:
        j = 0;
        break;
      case 1465714303:
        j = 1;
        break;
      case 2025639739:
        j = 2;
        break;
      case 943026201:
        j = 3;
        break;
      case 7524595:
        j = 4;
        break;
      case 3483829447:
        j = 5;
        break;
      case 4249632375:
        j = 6;
        break;
      case 1768862521:
        j = 7;
        break;
      case 1651564235:
        j = 8;
        break;
      case 1895093031:
        j = 9;
        break;
    }

    if (i == 1) { //First digit is in the 8th column
      lcd.setCursor(8, 1);
      tens = j; //character is = to the tens digit
      lcd.print(tens);
    }

    if (i == 2) //Second digit is in the 9th column
    {
      lcd.setCursor(9, 1); //answer to be printed in 9th column and second row
      ones = j; //character is = to the ones digit
      lcd.print(ones);
    }
    irrecv.enableIRIn(); //reenable IR receiver
    irrecv.resume(); //prepare for next IR signal
  }

  if (results.value == 3083026375) //if the reset button is pressed
  {
    i = 0; //reset i to 0 for answer resubmission
    lcd.setCursor(8, 1); //answer to be printed in 8th column and second row
    lcd.print("RE"); //This means reset
  }
}

void moveMotor() {
  myStepper.setSpeed(motorSpeed); //sets motor speed
  myStepper.step(stepsPerRevolution / 100); //moves motor
}

void loop() {
  timer = millis();
  while (timer + difficulty >= millis()) {
    //during the x amount of time that this function runs, it constantly checks for the IR signal
    //When x amount of time ms is reached, it does question posed, then runs loop again.
    receive(); //receive answer.
    finalAnswer = tens * 10 + ones;
    c = a * b; //establish product.
    if (finalAnswer == c) { //if the answer is correct...
      lcd.setCursor(13, 1);
      lcd.print("Yes");
      digitalWrite(2, HIGH); //green LED goes HIGH
      digitalWrite(3, LOW);  //red LED goes LOW
      moveMotor(); //move motor
    }
    else if (finalAnswer != c && finalAnswer != 0) { //if the answer is incorrect...
      lcd.setCursor(13, 1);
      lcd.print("oof");
      digitalWrite(3, HIGH); //red LED goes HIGH
      digitalWrite(2, LOW);  //green LED goes LOW
    }
    else if (finalAnswer == 0) { //before first answer is selected
      lcd.setCursor(13, 1);
      lcd.print("..."); //questions are loading up.
    }
  }
  //These factors are random characters between 1 and 9
  a = random(1, 9);
  b = random(1, 9);

  //Question display:
  lcd.setCursor(0, 0);
  lcd.print("Question:");
  lcd.print(" "); //space
  lcd.print(a);
  lcd.print("*"); //multiplied by
  lcd.print(b);
  lcd.print("="); //is equal to
  lcd.setCursor(0, 1);
  lcd.print("Answer:"); //Answer is in the next column
}
