// Setup: 
//  initialize Serial port
//  Set pin 6 to output
// Loop: Listen for message.  When we get the message, turn on LED on pin 6

int LEDPin = 6;
int incomingByte; // for incoming serial data
int switchState = 0;
int previousSwitchState = 0;

void setup() {
  pinMode(LEDPin, OUTPUT);
  pinMode(2, INPUT);
  digitalWrite(LEDPin, LOW);

  Serial.begin(9600);
}

void loop() {
  // Read button press
  switchState = digitalRead(2);

  if (switchState == HIGH && switchState != previousSwitchState) {
    // If the switchState is pressed and it didn't used to be, send message to server
    Serial.write(1);
    previousSwitchState = switchState;
  } else if (switchState == LOW && switchState != previousSwitchState) {
    Serial.write(0);
    previousSwitchState = switchState;
  }

  // Check if we have a message available
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    if (incomingByte == 1) {
      digitalWrite(6, HIGH);
    } else {
      digitalWrite(6, LOW);
    }
  }
}
