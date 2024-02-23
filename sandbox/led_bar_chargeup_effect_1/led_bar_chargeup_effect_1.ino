const int maxBrightness =  255;
const int nTimeSteps = 10;
const int delayTime =  50;
const float aCoeff = maxBrightness / pow(nTimeSteps, 2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (int p=2; p<=11; p++) {
    analogWrite(p, 0);
  }

  for (int p=2; p<=11; p++) {
    for (int t=0; t<=nTimeSteps; t++) {
      int brightness = (int)(aCoeff * pow(t, 2));
      Serial.println(brightness);
      if (brightness > maxBrightness) {
        Serial.println("Max brightness exceeded!!!");
        brightness = maxBrightness;
      }
      analogWrite(p, brightness);
      delay(delayTime);
    }
    Serial.println();
  }
  delay(1000);
}
