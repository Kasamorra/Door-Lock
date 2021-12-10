//Password protected door lock
//Components: LCD, Keypad, Servo motor, red LED, green LED
//PINS LCD: 7, 8, 9, 10, 11, 12
//PINS KEYPAD: 2, 3, A0, A1, A2, A3, A4, A5
//PINS LEDs: 5, 6
//PIN SERVO: 13

//Libraries
#include <Key.h> //Keypad Library
#include <Keypad.h> //Keypad Library
#include <Servo.h> //Servo motor library
#include<LiquidCrystal.h> //library for the LCD

//LCD
const int rs = 7;
const int en = 8;
const int d4 = 9;
const int d5 = 10;
const int d6 = 11;
const int d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Keypad
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char hexakeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, A5, A4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A3, A2, A1, A0}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexakeys), rowPins, colPins, ROWS, COLS );
char customKey;

//LEDs
const int greenPin = 5; //Pin for green LED
const int redPin = 6; //Pin for red LED

//Servo
const int servoPin = 13; //Pin connected to servo motor
const int servoOpen = 90; //Angle of Servo when lock open
const int servoClosed = 0; //Angle of Servo when lock closed
Servo myServo; //creates an object of type Servo

//Other
const int codeLength = 3; //length of password
char pwSet[codeLength] = {'3', '7', '3'}; //correct password: 373
char pwEntered[codeLength] = {'0', '0', '0'}; //password entered by user, initialized as 000
int arrayCounter = 0; //used to navigate through the array
int threeStrikes = 0; //later used to check if all entered digits match the ones of the password
const int delayTime = 1000; //delay of one second
int timer = 10; //time in between tries
int timerLong = 60; //time after 5 incorrect tries
int counterAttempts = 5; //counter to keep track of wrong attempts


void setup() {
  //LCD
  lcd.begin(16, 2);

  //LEDs
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);

  //Servo
  myServo.attach(servoPin);
}


void loop() {

  //Initial Reset
  digitalWrite(redPin, HIGH); //red LED means door locked
  digitalWrite(greenPin, LOW);
  timer = 10;
  arrayCounter = 0;
  threeStrikes = 0;
  myServo.write(servoClosed); //closes lock
  lcd.clear();

  //Prompting User to enter password
  lcd.setCursor(0, 0); //sets cursor to top left corner
  lcd.print("Enter Password");
  lcd.setCursor(0, 1); //sets cursor to bottom left corner
  lcd.print("Attempts: ");
  lcd.print(counterAttempts);

  //Reading entered password
  lcd.setCursor(0, 1);

  while (arrayCounter < 3) //saves input in the right char
  {
    customKey = customKeypad.getKey();
    if (customKey)
    {
      if (arrayCounter == 0) //used to update screen when first digit is entered
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter Password");
        lcd.setCursor(0, 1);
      }
      pwEntered[arrayCounter] = customKey; //saves entered key in array
      lcd.print(pwEntered[arrayCounter]); //prints entered key
      arrayCounter = arrayCounter + 1;
    }
  }

  delay(delayTime);

  //Digit-by-digit comparison with correct password
  for (arrayCounter = 0; arrayCounter < 3; arrayCounter++)
  {
    if (pwSet[arrayCounter] == pwEntered[arrayCounter])
      threeStrikes = threeStrikes + 1; //increases value only when compared digit is correct
  }

  //In case all digits were entered correctly
  if (threeStrikes == 3)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
    lcd.print("You may enter");
    myServo.write(servoOpen); //servo opens the lock
    counterAttempts = 5; //resets attempts
  }
  else //In case wrong password was entered
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    counterAttempts = counterAttempts - 1;
    lcd.print("Wrong Password!");
  }

  //Starting right timer
  if (counterAttempts < 1) //pw was entered incorrectly 5 times
  {
    while (timerLong >= 0)
    {
      lcd.setCursor(0, 1);
      
      if (timerLong < 10)
        lcd.print("0");

      lcd.print(timerLong);
      timerLong = timerLong - 1;
      delay(delayTime);
    }
    counterAttempts = 5;
  }

  else //pw was entered incorrectly less than 5 times
  {
    while (timer >= 0)
    {
      lcd.setCursor(0, 1);
      
      if (timer < 10)
        lcd.print("0");

      lcd.print(timer);
      timer = timer - 1;
      delay(delayTime);
    }
  }

} //void loop
