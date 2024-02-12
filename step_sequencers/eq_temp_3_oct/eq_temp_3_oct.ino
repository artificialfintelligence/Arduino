/* 
Code originally from https://www.hackster.io/eEdizon/arduino-step-sequencer-afea0e

Licensed under © GPL3+
*/

const byte speaker = 9;  //  Speaker pin
const byte pat0 = 4;      //  Led pins
const byte pat1 = 5;
const byte pat2 = 6;
const byte pat3 = 7;

bool debug = true; // Used to print values to Serial Monitor
bool isOn = false;  // Checks if the device should work or not
long counter = 0;   // Used for speed
long tempo;         // Used for reading the speed
int lowFreq = 10;    // The lower frequency boundary
int highFreq = 1033;// The higher frequency boundary
int pattern = 0;    // Current potentiometer
unsigned int note;  // Note that will be played

void switchLEDs(int pattern) {  // Turn on and off the leds for specific patterns
  switch (pattern) {
    case 0:
      digitalWrite(pat3, LOW);
      digitalWrite(pat0, HIGH);
      break;
    case 1:
      digitalWrite(pat0, LOW);
      digitalWrite(pat1, HIGH);
      break;
    case 2:
      digitalWrite(pat1, LOW);
      digitalWrite(pat2, HIGH);
      break;
    case 3:
      digitalWrite(pat2, LOW);
      digitalWrite(pat3, HIGH);
      break;
  }
}

float getNote(int pattern) {  //  Get notes for specific patterns
  int semitones = 0;
  float note = 0.0;
  if (pattern == 0) {
    note = 1.0 * map(analogRead(A0), 0, 1023, lowFreq, highFreq);
  } else if (pattern == 1) {
    semitones = map(analogRead(A1), 0, 1023, 0, 37);
    if (semitones != 0) {
      note = getNote(0) * pow(2, (semitones - 1) / 12.0);
    }
  } else if (pattern == 2) {
    semitones = map(analogRead(A2), 0, 1023, 0, 37);
    if (semitones != 0) {
      note = getNote(0) * pow(2, (semitones - 1) / 12.0);
    }
  } else if (pattern == 3) {
    semitones = map(analogRead(A3), 0, 1023, 0, 37);
    if (semitones != 0) {
      note = getNote(0) * pow(2, (semitones - 1) / 12.0);
    }
  }
  return note;
}


void setup() {
  pinMode(pat0, OUTPUT);
  pinMode(pat1, OUTPUT);
  pinMode(pat2, OUTPUT);
  pinMode(pat3, OUTPUT);
  digitalWrite(pat0, LOW);
  digitalWrite(pat1, LOW);
  digitalWrite(pat2, LOW);
  digitalWrite(pat3, LOW);

  if (debug) {
    Serial.begin(9600);
  }
}


void loop() {

  tempo = map(analogRead(A4), 0, 1023, 4000, 100);
  if (tempo < 3900) { // Enter if speed still acceptable
    if (counter > tempo) {
      noTone(speaker);
      counter = 0;

      switchLEDs(pattern);

      float note = getNote(pattern);
      if (debug) {
        Serial.print("Note ");
        Serial.print(pattern);
        Serial.print(": ");
        Serial.print(note, 3);
        Serial.print("Hz   ");
        if (pattern == 3) {
          Serial.println();
        }
      }

      pattern = (pattern + 1) % 4;
      if (note > lowFreq + 10) {
        tone(speaker, note);
      }
    }
    counter++;
  } else {  // Stopped on a specific potentiometer
    switchLEDs(pattern);
    float note = getNote(pattern);

    if (note > lowFreq + 10) {
      tone(speaker, note);
    } 
    else {
      noTone(speaker);
    }
  }
}