// Easily my dog's least favorite project

// Setup
//  initialize values

// Main loop
//  Read from photosensor
//  Adjust frequency of piezo

int sensorValue;
int sensorLow = 1023;
int sensorHigh = 0;

const int ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  while (millis() < 5000) {
    // For first five seconds, initialize low and high values
    sensorValue = analogRead(A0);
    if (sensorValue < sensorLow) {
      sensorLow = sensorValue;
    }
    if (sensorValue > sensorHigh) {
      sensorHigh = sensorValue;
    }
  }
  digitalWrite(ledPin, LOW);
}

void loop() {
  sensorValue = analogRead(A0);

  int pitch = map(sensorValue, sensorLow, sensorHigh, 50, 1000);

  // tone(pin, frequency in hz, duration in ms)
  tone(8, pitch, 20);

  delay(10);
}