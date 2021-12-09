//Step 4: Allowing only a certain amount of attempts
//Still using Serial instead of LCD
//PINS KEYPAD: 2, 3, 4, 5, 6, 7, 8, 9
//PINS LEDs: 11, 12
//PIN SERVO: 13

#include <Key.h> //Keypad Library
#include <Keypad.h> //Keypad Library
#include <Servo.h> //Servo motor library

//Keypad
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char hexakeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexakeys), rowPins, colPins, ROWS, COLS );
char customKey;

//Password
const int codeLength = 3;
char pwSet[codeLength] = {'2', '3', '8'}; //password as set by us
char pwEntered[codeLength] = {'0', '0', '0'}; //entered password
int arrayCounter = 0; //variable used to navigate through array
int threeStrikes = 0; //later used to check if all three digits are correct

//LEDs
const int greenPin = 11; //Pin for green LED
const int redPin = 12; //Pin for red LED

//Servo
const int servoPin = 13; //Pin connected to servo motor
const int servoOpen = 90; //Angle of Servo when lock open
const int servoClosed = 0; //Angle of Servo when lock closed
Servo myServo; //creates an object of type Servo

//Other
const int delayTime = 1000;
int timer = 10;
int timerLong = 60;
const int counterAttempts = 5;

void setup() {
  Serial.begin(9600); //to substitute the LCD
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  myServo.attach(servoPin); //attaches the motor to the pin
}

void loop() {
  //Initial Reset
  digitalWrite(redPin, HIGH); //red LED means door locked
  digitalWrite(greenPin, LOW);
  timer = 10; //resets the timer
  arrayCounter = 0; //resets the array Counter
  threeStrikes = 0; //resets the Strike Counter
  myServo.write(servoClosed); //resets the servo position

  Serial.print("Enter Code: ");

  while (arrayCounter < 3) //used to save the input in the right char
  {
    customKey = customKeypad.getKey();
    if (customKey)
    {
      pwEntered[arrayCounter] = customKey; //saves entered key in array
      Serial.print(pwEntered[arrayCounter]); //prints entered key
      arrayCounter = arrayCounter + 1;
    }
  }


  Serial.println(); //Leerzeile
  delay(delayTime);

  for (arrayCounter = 0; arrayCounter < 3; arrayCounter++)
  {
    if (pwSet[arrayCounter] == pwEntered[arrayCounter])
      threeStrikes = threeStrikes + 1;
  }

  if (threeStrikes == 3)
  {
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
    Serial.println("COME IN HERE!!!!");
    myServo.write(servoOpen); //servo opens the lock
  }
  else
  {
    counterAttempts = counterAttempts - 1;
    Serial.print("Wrong!! - ");
    Serial.print(counterAttempts);
    Serial.println(" attepmts left");
  }

  if (counterAttempts < 1)
  {
    while (timerLong >= 0)
    {
      if (timerLong < 10)
        Serial.print("0");
      Serial.println(timerLong);
      timerLong = timerLong - 1;
      delay(delayTime);
    }
  }
  else
  {
    while (timer >= 0)
    {
      if (timer < 10)
        Serial.print("0");
      Serial.println(timer);
      timer = timer - 1;
      delay(delayTime);
    }
  }

}
