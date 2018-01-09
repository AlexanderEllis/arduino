/*
  This is an application to perform an action based on a number of knocks.

  The piezo is used to detect vibration.  Based on its input, a knock will register as
  either a loud knock or a soft knock.  The code is currently set to listen for 3 loud knocks,
  at which point it will rotate the Servo motor.  The switch can then be used to "relock" the servo.


  setup():
    Initialize input and output pins

  loop():
    Check if switch is pressed
      If it is, "lock" servo
    Otherwise, check for piezo input
      If knock, update knocks remaining
      If reached the total number of needed knocks, unlock piezo

  checkForKnock(piezoInput):
    If piezoInput is in our allowable knock range, it's a good knock
    Otherwise it's a bad knock
*/

#include <Servo.h>
Servo myServo;

const int piezo = A0;
const int switchPin = 2;
const int yellowLed = 3;
const int greenLed = 4;
const int redLed = 5;

int knockVal;
int switchVal;

const int quietKnock = 10;
const int loudKnock = 100;

boolean locked = false;
int numberOfKnocks = 0;

void setup() {
  myServo.attach(9);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(switchPin, INPUT);
  Serial.begin(9600);
  myServo.write(0);
  Serial.println("The box is unlocked!");
}

void loop() {
  if (locked == false) {
    switchVal = digitalRead(2);
    if (switchVal == HIGH) {
      locked = true;
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      myServo.write(90);
      Serial.println("The box is locked.");
      delay(1000);
    }
  } else if (locked == true) {
    knockVal = analogRead(piezo);

    if (numberOfKnocks < 3 && knockVal > 0) {
      if (checkForKnock(knockVal) == true) {
        numberOfKnocks++;
      }
      Serial.print(3 - numberOfKnocks);
      Serial.println(" more knocks to go");
    }
    if (numberOfKnocks >= 3) {
      locked = false;
      myServo.write(0);
      delay(20);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
      Serial.println("The box is unlocked!");
      numberOfKnocks = 0;
    }
  }
}

boolean checkForKnock(int value) {
  if (value > quietKnock && value < loudKnock) {
    digitalWrite(yellowLed, HIGH);
    delay(50);
    digitalWrite(yellowLed, LOW);
    Serial.print("Valid knock of value ");
    Serial.println(value);
    return true;
  } else {
    Serial.print("Bad knock value ");
    Serial.println(value);
    return false;
  }
}
