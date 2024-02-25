#include "LedControl.h"

LedControl lc = LedControl(12, 11, 10, 1);

enum DirH { rl, lr };
enum DirV { ud, du };

const byte heart_solid[8] = {
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
  lc.setIntensity(0, 8);
  /* and clear the display */
  lc.clearDisplay(0);
}

void loop() {
  int d = 50;
  bool s = 1;
  
  lc.clearDisplay(0);
  displayCol(B01100110, d);
  
  lc.clearDisplay(0);
  displayRow(B10011001, d);

  lc.clearDisplay(0);
  SweepLineV(3, 3, 3, s, d);
  SweepLineH(2, 3, 4, s, d);
  SweepLineV(5, 2, 4, s, d);
  SweepLineH(5, 5, 2, s, d);
  SweepLineV(1, 5, 1, s, d);
  SweepLineH(0, 1, 6, s, d);
  SweepLineV(7, 0, 6, s, d);
  SweepLineH(7, 7, 0, s, d);

  SweepLineV(0, 0, 6, s, d);
  SweepLineH(6, 1, 6, s, d);
  SweepLineV(6, 5, 1, s, d);
  SweepLineH(1, 5, 2, s, d);
  SweepLineV(2, 2, 4, s, d);
  SweepLineH(4, 3, 4, s, d);
  SweepLineV(4, 3, 3, s, d);

  for (int i = 3; i >= 0; i--) {
    DrawSolidSquare(i, i, 7 - i, 7-i, s, 4 * d);
  }
  
  for (int k = 2; k <= 10; k++) {
    lc.clearDisplay(0);
    delay(pow(2, k));
    DisplayPic(heart_solid, pow(2, k));
  }
  
  
  // Scroll from top edge to bottom edge
  ScrollV(smile, d, DirV::ud);
  // scroll from bottom edge to top edge
  ScrollV(smile, d, DirV::du);

  // Scroll from left edge to right edge
  ScrollH(heart_hollow, d, DirH::lr);
  // scroll from right edge to left edge
  ScrollH(heart_hollow, d, DirH::rl);

  // Left to right while bobbing up and down
  BobScrollH(smile_up, smile_down, 2 * d, lr);
  BobScrollH(smile_up, smile_down, 2 * d, rl);
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

  for (int k = 0; k <= 16; k++) {
    for (int i = 0; i < 8; i++) {
    // Determine the shift operation based on the direction
    if (dir == rl) {
        frame[i] = k <= 8 ? pic[i] >> (8 - k) : pic[i] << (k - 8);
      } else {
        frame[i] = k <= 8 ? pic[i] << (8 - k) : pic[i] >> (k - 8);
      }
    }
    DisplayPic(frame, d);
  }
}

void BobScrollH(byte pic_1[8], byte pic_2[8], int d, DirH dir) {
  byte frame[8] = {};
  byte *pic; // Pointer to the current array

  for (int k =  0; k <= 16; k++) {
    // Alternate between pic_1 and pic_2 every other iterations
    pic = ((pic == pic_1) ? pic_2 : pic_1);
    for (int i =  0; i <  8; i++) {
      // Determine the shift operation based on the direction
      if (dir == rl) {
        frame[i] = k <= 8 ? pic[i] >> (8 - k) : pic[i] << (k - 8);
      } else {
        frame[i] = k <= 8 ? pic[i] << (8 - k) : pic[i] >> (k - 8);
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

void SweepLineH(int y, int x_start, int x_end, bool turn_on, int d) {
  int x = x_start;
  bool done = !(y >= 0 && y < 8 && x_start >= 0 && x_start < 8 && x_end >= 0 && x_end < 8);
  while (!done) {
    lc.setLed(0, x, y, turn_on);
    delay(d);
    x += (x_start <= x_end ? 1 : -1);
    done = ((x_start <= x_end && x > x_end) || (x_start >= x_end && x < x_end));
  }
}

void SweepLineV(int x, int y_start, int y_end, bool turn_on, int d) {
  int y = y_start;
  bool done = !(x >= 0 && x < 8 && y_start >= 0 && y_start < 8 && y_end >= 0 && y_end < 8);
  while (!done) {
    lc.setLed(0, x, y, turn_on);
    delay(d);
    y += (y_start <= y_end ? 1 : -1);
    done = ((y_start <= y_end && y > y_end) || (y_start >= y_end && y < y_end));
  }
}

void DrawSolidSquare(int top_l_x, int top_l_y, int bot_r_x, int bot_r_y, bool inverted, int d) {
  // Validate args
  if (top_l_x < 0 || top_l_y < 0 || bot_r_x >= 8 || bot_r_y >= 8) {
      return;
  }

  // Generate binary array
  byte bin_arr[8];
  // Initialize the binary array with all zeros (ones)
  for (int i = 0; i < 8; ++i) {
    bin_arr[i] = (inverted ? 0xff : 0x00);
  }

  // Set the bits inside the square to 1 (0)
  for (int i = top_l_y; i <= bot_r_y; ++i) {
    for (int j = top_l_x; j <= bot_r_x; ++j) {
      if (inverted) {
        bitClear(bin_arr[i], 7 - j);
      }
      else {
        bitSet(bin_arr[i], 7 - j);
      }
    }
  }
  DisplayPic(bin_arr, d);
}

void displayCol(byte data[8], int d) {
  lc.clearDisplay(0);
  for (int col = 0; col < 8; col++) {
    lc.setColumn(0, col, data);
    delay(d);
  }
}

void displayRow(byte data[8], int d) {
  lc.clearDisplay(0);
  for (int row = 0; row < 8; row++) {
    lc.setRow(0, row, data);
    delay(d);
  }
}