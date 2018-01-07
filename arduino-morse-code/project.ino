/*
  This is a morse code reader.
  
  It assumes that letters will have at least a second pause in between and words will have at least a 2 second pause.  It assumes a dash will be longer than 1 second.

  If it receives a pause longer than 4 seconds, it will send the message to the serial port.
  Future: Print to screen?

  Requirements:
    You can send a dot to the arduino by holding down the button for less than one second.
    You can send a dash to the arduio by holding down the button for at least a second.
    You can signal that you're in between words by pausing for two seconds.
      LED would be handy here
    You can signal that you've finished the word by pausing for four seconds.
    For now, you can send up to 60 char

  Psuedocode:

  prevSwitchState = 0;
  switchState = 0;
  inputStartTime = 0;
  inputEndTime = 0;
  sentence = array of chars
  currentMorse = array of dots/dashes

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
        If sentence isn't empty and time is longer than 3 seconds, we're in between words
          Add space to sentence
        If sentence isn't empty and time is longer than 5 seconds, we're done with this sentence.
          Add period to sentence.
          Reset sentence.
  
  dotOrDash(inputInterval):
    If inputInterval < 1 second,
      return dot
    else
      return dash
  
  morseParser(morseString):
    switch(morseString):
      case '.':
        return "e"
        break;
      case '-':
        return "t";
        break
      etcetera

  Edge cases:
    morse code longer than 5
    sentence longer than 60 char
*/

int switchPin = 2;
int prevSwitchState = 0;
int switchState;
char sentence[60];
int sentenceLength = 0;
char currentMorse[5];
int currentMorseLength = 0;
unsigned long currentTime;
unsigned long inputStartTime = 0;
unsigned long inputEndTime = 0;
unsigned long timeSinceLastInput = 0;

char * morseInputs[] = {
  ".-", // A
  "-...", // B
  "-.-.", // C
  "-..", 
  ".", 
  "..-.", 
  "--.",
  "....",
  "..",
  ".---",
  "-.-",
  ".-..",
  "--",
  "-.",
  "---",
  ".--.",
  "--.-",
  ".-.",
  "...", // S
  "-",
  "..-",
  "..-.",
  "-..-",
  "-.--",
  "--..", // Z
  "-----", // 0
  ".----", // 1
  "..---",
  "...--",
  "....-",
  ".....",
  "-....",
  "--...",
  "---..",
  "----.", // 9
};

void setup() {
  pinMode(switchPin, INPUT);
  Serial.initialize(9600);
}

void loop() {
  switchState = digitalRead(switchPin);
  currentTime = millis();

  if (switchState == HIGH && prevSwitchState == LOW) {
    inputStartTime = currentTime;
  } else if (switchState == LOW && prevSwitchState == HIGH) {
    inputEndTime = currentTime;
    int inputInterval = inputEndTime - inputStartTime;
    char morseInput = dotOrDash(inputInterval);
    currentMorse[currentMorseLength] = morseInput;
    currentMorseLength++;
  } else if (switchState == LOW && prevSwitchState == LOW) {
    timeSinceLastInput = currentTime - inputEndTime;
    if (currentMorseLength != 0 && timeSinceLastInput > 1000) {
      // We're between letters
      char inputLetter = morseParser(currentMorse);
      sentence[sentenceLength] = inputLetter;
      sentenceLength++;
      memset(currentMorse, 0, sizeof(currentMorse));
    } else if (sentenceLength != 0 && timeSinceLastInput > 3000) {
      // We're in between words
      sentence[sentenceLength] = ' ';
      sentenceLength++;
    } else if (sentenceLength != 0 && timeSinceLastInput > 5000) {
      // We're in between sentences.
      sentence[sentenceLength] = '.';
      Serial.println(sentence);
      memset(sentence, 0, sizeof(sentence));
      sentenceLength = 0;
    }
  }

  prevSwitchState = switchState;
}

char morseParser(char inputString[]) {
  for (int i = 0; i < 35; i++) {
    if (strcmp(inputString, morseInputs[i]) == 0) {
      if (i < 25) {
        return 65 + i;
      } else {
        return 23 + i;
      }
    }
  }
}

char dotOrDash(unsigned long inputInterval) {
  if (inputInterval < 1000) {
    return '.';
  } else {
    return '-';
  }
}
