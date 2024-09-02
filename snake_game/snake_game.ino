#include "LedControl.h"
#include "snake.h"
#include "point.h"
#include "food.h"

// /* Joystick Defs */
const int jsX = A1; // Joystick X pin
const int jsY = A2; // Joystick Y pin
const int jsP = 8;  // Joystick button press pin

/* LED Matrix Defs */
const int ledDat = 12;
const int ledClk = 11;
const int ledCS = 10;
LedControl lc = LedControl(ledDat, ledClk, ledCS, 1);

/* Potentiometer for Speed Adjustment */
int speedValue; // = analogRead(A3);

/* Global Vars */
const unsigned long min_interval = 100;
const unsigned long max_interval = 1000;

unsigned long prevMillis = 0;
unsigned long interval = 500;

unsigned long prevMillis_blink = 0;
unsigned long interval_blink = 100;
bool head_led_status = true;

unsigned long prevMillis_blink_food = 0;
unsigned long interval_blink_food = 250;
bool food_led_status = true;

char in_byte;
Snake snake;
Snake::Direction dir;
Food food;
bool do_grow;
bool game_over;

/* Function for fancy Game-Over animations */
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

void DisplayPic(byte pic[8], int d) {
  lc.clearDisplay(0);
  for (int col = 0; col < 8; col++)
  {
    lc.setRow(0, col, pic[col]);
  }
  delay(d);
}

void seedRandom() {
    // Read from an unconnected analog pin to get a random seed
    randomSeed(analogRead(A0));
}

void resetGame() {
  lc.clearDisplay(0);

  /*
    A crucially important idea is that we NEVER directly give "raw" coordinates to any of the
    classes. On the other hand, they automatically handle conversions, so any coordinates we *get*
    from them are already converted to the correct coordinates for LedControl to work with.
  */
  snake = Snake();
  dir = snake.GetDir();
  food = Food();
  Point init_head = snake.GetChain().GetHead()->data;
  Point init_food = food.GetCoords();
  lc.setLed(0, init_head.x, init_head.y, true);

  // Make sure food is not on snake's head
  while (init_food.x == init_head.x && init_food.y == init_head.y) {
    food = Food();
    init_food = food.GetCoords();
  }
  lc.setLed(0, init_food.x, init_food.y, true);

  game_over = false;
}

void setup()
{
  pinMode(jsP, INPUT_PULLUP); // Set the joystick button press pin to INPUT_PULLUP

  /*
    The MAX72XX is in power-saving mode on startup, we have to do a wakeup call.
  */
  lc.shutdown(0, false);
  lc.setIntensity(0, 4);

  // Serial.begin(9600);

  resetGame();
}

void loop()
{
  if (!digitalRead(jsP)) {
    delay(200);
    resetGame();
  }
  speedValue = analogRead(A3);
  interval = max_interval - (speedValue / 1024.0) * (max_interval - min_interval);

  unsigned long currMillis = millis();

  if (game_over) {
    delay(3000);
    for (int i = 3; i >= 0; i--) {
      DrawSolidSquare(i, i, 7 - i, 7-i, 0, 200);
    }
    for (int i = 3; i >= 0; i--) {
      DrawSolidSquare(i, i, 7 - i, 7-i, 1, 200);
    }
    resetGame();
  }
  else {
    Point head_pos = snake.GetChain().GetHead()->data;
    Point food_loc = food.GetCoords();

    Snake::Direction curr_dir = snake.GetDir();
    if (analogRead(jsX) > 767) {
      if (curr_dir != Snake::Direction::left) {
        dir = Snake::Direction::right;
      }
    }
    if (analogRead(jsX) < 255) {
      if (curr_dir != Snake::Direction::right) {
        dir = Snake::Direction::left;
      }
    }
    if (analogRead(jsY) > 767) {
      if (curr_dir != Snake::Direction::up) {
        dir = Snake::Direction::down;
      }
    }
    if (analogRead(jsY) < 255) {
      if (curr_dir != Snake::Direction::down) {
        dir = Snake::Direction::up;
      }
    }

    if (currMillis - prevMillis_blink >= interval_blink && curr_dir != Snake::Direction::none) {
      prevMillis_blink = currMillis;
      head_led_status = !head_led_status;
      lc.setLed(0, head_pos.x, head_pos.y, head_led_status);
    }

    if (currMillis - prevMillis_blink_food >= interval_blink_food) {
      prevMillis_blink_food = currMillis;
      food_led_status = !food_led_status;
      lc.setLed(0, food_loc.x, food_loc.y, food_led_status);
    }
    
    if (currMillis - prevMillis >= interval) {
      prevMillis = currMillis;
      if (head_led_status == 0) {
        head_led_status = 1;
      }
      lc.setLed(0, head_pos.x, head_pos.y, head_led_status);
      Point tail_pos = snake.GetChain().GetTail()->data;

      snake.SetDir(dir);
      bool did_gow = snake.Update(food_loc);
      Point new_head_pos = snake.GetChain().GetHead()->data;
      if (snake.GetDir() != Snake::Direction::none && new_head_pos == head_pos) {
        game_over = true;
      }
      else {  // Update LEDs
        lc.setLed(0, new_head_pos.x, new_head_pos.y, 1);
        if (!did_gow && snake.GetDir() != Snake::Direction::none && new_head_pos != tail_pos) {
          lc.setLed(0, tail_pos.x, tail_pos.y, 0);
        }
        if (did_gow) {
          food.Spawn(snake.GetChain());
          food_loc = food.GetCoords();
          lc.setLed(0, food_loc.x, food_loc.y, 1);
        }
      }
    }
  } 
}
