#include "LedControl.h"

LedControl lc = LedControl(12, 11, 10, 1);

enum DirH { rl, lr };
enum DirV { ud, du };

const byte heart_full[8] = {
  B00000000,
  B01100110,
  B11111111,
  B11111111,
  B01111110,
  B00111100,
  B00011000,
  B00000000
};

const byte heart_hollow[8] = {
  B00000000,
  B01100110,
  B10011001,
  B10000001,
  B01000010,
  B00100100,
  B00011000,
  B00000000
};

const byte smile[8] = {
  B00000000,
  B00100100,
  B00100100,
  B00100100,
  B10000001,
  B11000011,
  B01100110,
  B00111100
};

const byte smile_up[8] = {
  B00100010,
  B01110111,
  B01010101,
  B00000000,
  B01000001,
  B01100011,
  B00111110,
  B00000000
};

const byte smile_down[8] = {
  B00000000,
  B00100010,
  B01110111,
  B01010101,
  B00000000,
  B01000001,
  B01100011,
  B00111110,
};

void setup() {
  /*
    The MAX72XX is in power-saving mode on startup,
    we have to do a wakeup call
  */
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 5);
  /* and clear the display */
  lc.clearDisplay(0);
}

void loop() {
  DisplayPic(heart_full, 1000);
  
  // scroll from left edge to right edge
  ScrollH(heart_hollow, 50, DirH::lr);
  // scroll from right edge to left edge
  ScrollH(heart_hollow, 50, DirH::rl);

  // scroll from top edge to bottom edge
  ScrollV(smile, 50, DirV::ud);
  // scroll from bottom edge to top edge
  ScrollV(smile, 50, DirV::du);

  // Left to right while bobbing up and down
  BobScrollH(smile_up, smile_down, 100, lr);
  BobScrollH(smile_up, smile_down, 100, rl);


}

void DisplayPic(byte pic[8], int d) {
  lc.clearDisplay(0);
  for (int col = 0; col < 8; col++)
  {
    lc.setRow(0, col, pic[col]);
  }
  delay(d);
}

void ScrollH(byte pic[8], int d, DirH dir) {
  byte frame[8] = {};

  for (int k = 0; k < 16; k++) {
    for (int i = 0; i < 8; i++) {
    // Determine the shift operation based on the direction
    if (dir == rl) {
        frame[i] = k <  8 ? pic[i] >> (8 - k) : pic[i] << (k -  8);
      } else {
        frame[i] = k <  8 ? pic[i] << (8 - k) : pic[i] >> (k -  8);
      }
    }
    DisplayPic(frame, d);
  }
}

void BobScrollH(byte pic_1[8], byte pic_2[8], int d, DirH dir) {
  byte frame[8] = {};
  byte *pic; // Pointer to the current array

  for (int k =  0; k <  16; k++) {
    // Alternate between pic_1 and pic_2 every other iterations
    pic = ((pic == pic_1) ? pic_2 : pic_1);
    for (int i =  0; i <  8; i++) {
      // Determine the shift operation based on the direction
      if (dir == rl) {
        frame[i] = k <  8 ? pic[i] >> (8 - k) : pic[i] << (k -  8);
      } else {
        frame[i] = k <  8 ? pic[i] << (8 - k) : pic[i] >> (k -  8);
      }
    }
    DisplayPic(frame, d);
  }
}

void ScrollV(byte pic[8], int d, DirV dir) {
  byte frame[8] = {};

  // Step 1: Scroll in from one edge to the middle
  if (dir == ud) {
    for (int k = 0; k < 8; k++) {
      for (int i = 0; i <= k; i++ ) {
        frame[i] = pic[7 - k + i];
      }
      DisplayPic(frame, d);
    }
  }
  else {
    for (int k = 7; k >= 0; k--) {
      for (int i = k; i < 8; i++) {
        frame[i] = pic[i - k];
      }
      DisplayPic(frame, d);
    }
  }

  // Step 2: Scroll out from the middle to the other edge
  if (dir == ud) {
    for (int k = 8; k > 0; k--) {
      lc.clearDisplay(0);
      for (int i = 9 - k; i < 8; i++ ) {
        frame[i] = pic[(i + k) % 9];
      }
      DisplayPic(frame, d);
    }
  }
  else {
    for (int k = 0; k < 8; k++) {
      lc.clearDisplay(0);
      for (int i = 0; i <= 7 - k; i++ ) {
        frame[i] = pic[i + k + 1];
      }
      DisplayPic(frame, d);
    }
  }
}