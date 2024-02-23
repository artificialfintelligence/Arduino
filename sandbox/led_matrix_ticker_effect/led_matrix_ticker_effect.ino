#include "LedControl.h"

LedControl lc = LedControl(12, 11, 10, 1);

void setup() {
  /*
    The MAX72XX is in power-saving mode on startup,
    we have to do a wakeup call
  */
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 1);
  /* and clear the display */
  lc.clearDisplay(0);
  Serial.begin(9600);
}

void loop() {
  byte heart[8] = {
    B00000000,
    B01100110,
    B11111111,
    B11111111,
    B01111110,
    B00111100,
    B00011000,
    B00000000
  };

  byte pic[8] = {};
  for (int k = 8; k > 0; k--) {
    for (int i = 0; i < 8; i++) {
      pic[i] = heart[i] >> k;
    }
    displayPic(pic);
    delay(100);
  }
  pic[8] = {};
  for (int k = 0; k < 8; k++) {
    for (int i = 0; i < 8; i++) {
      pic[i] = pic[i] << 1;
      pic[i] = pic[i] | heart[i] << k;
    }
    displayPic(pic);
    delay(100);
  }
  // for (int k = 0; k < 8; k++) {
  //   for (int i = 0; i < 8; i++) {
  //     heart[i] = heart[i] << 1;
  //   }
  //   displayPic(heart);
  //   delay(100);
  // }

//   int c = 0;
//   for (int d = 4; d <= 2048; d *= 2) {
//     displayPic(heart);
//     c++;
//     delay(d);
//     Serial.print(c);
//     Serial.print(" ");
//     Serial.println(d);
//   }
}

void displayDot() {
  lc.clearDisplay(0);
  delay(100);
  for (int row = 0; row < 8; row++)
  {
    for (int col = 0; col < 8; col++)
    {
      lc.setLed(0, row, col, true);
      delay(50);
    }
  }
}

void displayCol() {
  lc.clearDisplay(0);
  delay(100);
  byte data = B01100110;
  for (int col = 0; col < 8; col++) {
    lc.setColumn(0, col, data);
    delay(100);
  }
}

void displayRow() {
  lc.clearDisplay(0);
  delay(100);
  byte data = B10011001;
  for (int row = 0; row < 8; row++) {
    lc.setRow(0, row, data);
    delay(100);
  }
}

void displayPic(byte pic[8]) {
  lc.clearDisplay(0);
  // delay(100);
  for (int col = 0; col < 8; col++)
  {
    lc.setRow(0, col, pic[col]);
  }
}
