/*
  Project 8 is creating a digital hourglass.  

  Pins 2-7 are LEDs that will display the current status of the hourglass as it counts up.
  Every 5 seconds, another LED will illuminate.
  When we've reached the total time (30 seconds), the lights will flash and then the timer will start again.

  Pin 8 is connected to a tilt switch so that the hourglass will reset if it is flipped upside down.
*/
const int switchPin = 8;
unsigned long previousTime = 0;
int switchState = 0;
int prevSwitchState = 0;

int led = 2;
long interval = 5000; // Five seconds each step

void setup() {
  for (int x = 2; x < 8; x++) {
    pinMode(x, OUTPUT);
  }
  pinMode(switchPin, INPUT);
}

void turnAllTo(int powerLevel) {
  for (int x = 2; x < 8; x++) {
    digitalWrite(x, powerLevel);
  }
}

void resetTimer(int currentTime) {
  previousTime = currentTime;
  led = 2;
  turnAllTo(LOW);
}

void flashLights() {
  for (int i = 0; i < 10; i++) {
    turnAllTo(i % 2 ? HIGH : LOW);
    delay(200);
  }
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - previousTime > interval && led != 8) {
    previousTime = currentTime;
    digitalWrite(led, HIGH);
    led++;
  } else if (led == 8) {
    flashLights();
    resetTimer(currentTime);
  }

  switchState = digitalRead(switchPin);
  if (switchState != prevSwitchState) {
    resetTimer(currentTime);
  }
  prevSwitchState = switchState;
}
