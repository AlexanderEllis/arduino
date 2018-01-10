/*
  This is a morse code reader.
  
  It assumes that letters will have at least a second pause in between and words will have at least a 3 second pause.  It assumes a dash will be longer than 300 ms.

  If it receives a pause longer than 5 seconds, it will send the message to the serial port.
  It will also print to screen.
    TODO: Scroll a longer sentence

  Requirements:
    You can send a dot to the arduino by holding down the button for less than one second.
    You can send a dash to the arduio by holding down the button for at least a second.
    You can signal that you're in between words by pausing for two seconds.
      LED would be handy here
    You can signal that you've finished the word by pausing for five seconds.
    For now, you can send up to 60 char

  Psuedocode:

  setup():
    Initialize serial port
    Initialize port for button input
    Future: LED for status?

  loop():
    If button previously off and currently on:
      mark this time as start time for an input
    If button previously on and currently off:
      mark this time as end time for an input
      use times to calculate dot or dash
      add to current dot/dash array
    If button previously off and currently off:
      See how much time has passed since the last end time.
        If currentMorse isn't empty and time is longer than 1 seconds (for now), we're in between letters.
          create char from currentMorse and add to sentence
          Reset currentMorse
        If sentence isn't empty and time is longer than 5 seconds, we're done with this sentence.
          Add period to sentence.
          Reset sentence.
        If sentence isn't empty and time is longer than 3 seconds, we're in between words
          Add space to sentence
  
  dotOrDash(inputInterval):
    If inputInterval < 1 second,
      return dot
    else
      return dash
  
  morseParser(morseString):
    return match in morseCode array

  Edge cases:
    morse code longer than 5: return '?'
    TODO: sentence longer than 60 char
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 6);

int switchPin = 2;
int prevSwitchState = 0;
int switchState;
char sentence[60];
int sentenceLength = 0;
char currentMorse[6];
int currentMorseLength = 0;
unsigned long currentTime;
unsigned long inputStartTime = 0;
unsigned long inputEndTime = 0;
unsigned long timeSinceLastInput = 0;
boolean canAddSpace = true;


char * morseInputs[] = {
  ".-",    // A
  "-...",  // B
  "-.-.",  // C
  "-..",   // D
  ".",     // E
  "..-.",  // F
  "--.",   // G
  "....",  // H
  "..",    // I
  ".---",  // J
  "-.-",   // K
  ".-..",  // L
  "--",    // M
  "-.",    // N
  "---",   // O
  ".--.",  // P
  "--.-",  // Q
  ".-.",   // R
  "...",   // S
  "-",     // T
  "..-",   // U
  "...-",  // V
  ".--",   // W
  "-..-",  // X
  "-.--",  // Y
  "--..",  // Z
  "-----", // 0
  ".----", // 1
  "..---", // 2
  "...--", // 3
  "....-", // 4
  ".....", // 5
  "-....", // 6
  "--...", // 7
  "---..", // 8
  "----.", // 9
};

void setup() {
  pinMode(switchPin, INPUT);
  lcd.begin(16, 6);
  Serial.begin(9600);
}

void loop() {
  switchState = digitalRead(switchPin);
  currentTime = millis();

  if (switchState == HIGH && prevSwitchState == LOW) {
    inputStartTime = currentTime;
  } else if (switchState == LOW && prevSwitchState == HIGH) {
    inputEndTime = currentTime;
    int inputInterval = inputEndTime - inputStartTime;
    // Noticed some very short inputs of ~1-60ms inputs
    // I don't think I can press the button that fast!
    // Manual testing found a good minimum would be 80ms.
    if (inputInterval <= 80) {
      return;
    }
    char morseInput = dotOrDash(inputInterval);
    Serial.print("Morse input is: ");
    Serial.println(morseInput);
    currentMorse[currentMorseLength] = morseInput;
    currentMorseLength++;
  } else if (switchState == LOW && prevSwitchState == LOW) {
    timeSinceLastInput = currentTime - inputEndTime;
    if (currentMorseLength != 0 && timeSinceLastInput > 1000) {
      // We're between letters
      char inputLetter = morseParser(currentMorse);
      Serial.print("Received input letter: ");
      Serial.println(inputLetter);
      sentence[sentenceLength] = inputLetter;
      sentenceLength++;
      // Reset currentMorse array
      memset(currentMorse, 0, sizeof(currentMorse));
      currentMorseLength = 0;
      canAddSpace = true;
    } else if (sentenceLength != 0 && timeSinceLastInput > 5000) {
      // We're in between sentences.
      sentence[sentenceLength] = '.';
      Serial.println("Printing sentence");
      Serial.println(sentence);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(sentence);
      // Reset sentence array
      memset(sentence, 0, sizeof(sentence));
      sentenceLength = 0;
    } else if (sentenceLength != 0 && timeSinceLastInput > 3000 && canAddSpace == true) {
      // We're in between words
      Serial.println("Adding space");
      sentence[sentenceLength] = ' ';
      sentenceLength++;
      canAddSpace = false;
    }
  }

  prevSwitchState = switchState;
}

char morseParser(char inputString[]) {
  Serial.print("Parsing: \'");
  Serial.print(inputString);
  Serial.println("\'");
  for (int i = 0; i < 36; i++) {
    if (strcmp(inputString, morseInputs[i]) == 0) {
      if (i < 26) {
        return 65 + i;
      } else {
        return 22 + i;
      }
    }
  }
  return 63;
}

char dotOrDash(unsigned long inputInterval) {
  if (inputInterval < 300) {
    return '.';
  } else {
    return '-';
  }
}
