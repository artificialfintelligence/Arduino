const int ledPin = 9;
int ledStatus = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(2, INPUT);
}

void loop() {
  if (digitalRead(2) == 1)
  {
    delay(1); // debounce
    while (digitalRead(2) != 0) {
      // (void)0; noop and wait for button release 
    }
    ledStatus = !ledStatus;
    digitalWrite(ledPin, ledStatus);
  }
  delay(1);      
}
