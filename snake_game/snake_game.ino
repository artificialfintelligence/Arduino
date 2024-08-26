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

/* Global Vars */
unsigned long prevMillis = 0;
unsigned long interval = 500;

unsigned long prevMillis_blink = 0;
unsigned long interval_blink = 100;
bool head_led_status = true;

char in_byte;
Snake snake;
Food food;
bool do_grow;
bool game_over;

void seedRandom() {
    // Read from an unconnected analog pin to get a random seed
    randomSeed(analogRead(0));
}

void resetGame() {
  lc.clearDisplay(0);

  /*
    A crucially important idea is that we NEVER directly give "raw" coordinates to any of the
    classes. On the other hand, they automatically handle conversions, so any coordinates we *get*
    from them are already converted to the correct coordinates for LedControl to work with.
  */
  snake = Snake();
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

  Serial.begin(9600);

  resetGame();
}

void loop()
{
  unsigned long currMillis = millis();

  if (game_over) {
    while (digitalRead(jsP)) {
      delay (100);
    }
    resetGame();
  }
  else {
    Point head_pos = snake.GetChain().GetHead()->data;

    Snake::Direction curr_dir = snake.GetDir();
    Snake::Direction new_dir = curr_dir;
    if (analogRead(jsX) > 767) {
      if (curr_dir != Snake::Direction::left) {
        new_dir = Snake::Direction::right;
      }
    }
    if (analogRead(jsX) < 255) {
      if (curr_dir != Snake::Direction::right) {
        new_dir = Snake::Direction::left;
      }
    }
    if (analogRead(jsY) > 767) {
      if (curr_dir != Snake::Direction::up) {
        new_dir = Snake::Direction::down;
      }
    }
    if (analogRead(jsY) < 255) {
      if (curr_dir != Snake::Direction::down) {
        new_dir = Snake::Direction::up;
      }
    }
    snake.SetDir(new_dir);

    if (currMillis - prevMillis_blink >= interval_blink) {
      prevMillis_blink = currMillis;
      head_led_status = !head_led_status;
      lc.setLed(0, head_pos.x, head_pos.y, head_led_status);
    }
    
    if (currMillis - prevMillis >= interval) {
      prevMillis = currMillis;
      if (head_led_status == 0) {
        head_led_status = 1;
      }
      lc.setLed(0, head_pos.x, head_pos.y, head_led_status);
      Point tail_pos = snake.GetChain().GetTail()->data;
      Point food_loc = food.GetCoords();

      bool did_gow = snake.Update(food_loc);
      Point new_head_pos = snake.GetChain().GetHead()->data;
      if (snake.GetDir() != Snake::Direction::none && new_head_pos == head_pos) {
        Serial.write("Game Over!");
        Serial.write("\n");
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
