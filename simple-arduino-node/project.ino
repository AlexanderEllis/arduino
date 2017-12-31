/*
Setup: 
  Initialize I/O pins
  Initialize Serial port

Loop: 
  Check for button press change and send to server with Serial
  Check for incoming message and turn LED on/off accordingly
*/

int LEDPin = 6;
int incomingByte; // for incoming serial data
int switchState = LOW;
int previousSwitchState = LOW;

void setup() {
  pinMode(LEDPin, OUTPUT);
  pinMode(2, INPUT);

  // Start with LED turned off
  digitalWrite(LEDPin, LOW);

  Serial.begin(9600);
}

void loop() {
  // Read button press
  switchState = digitalRead(2);

  // Check previousSwitchState so we aren't constantly writing back and forth without a button change
  if (switchState != previousSwitchState) {
    Serial.write(switchState);
    previousSwitchState = switchState;
  }

  // Check if we have a message available
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    digitalWrite(6, incomingByte);
  }
}
