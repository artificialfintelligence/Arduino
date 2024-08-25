#include "LedControl.h"
#include "snake.h"
#include "point.h"
#include "food.h"

// /* Joystick Defs */
// const int jsX = A0; // Joystick X pin
// const int jsY = A1; // Joystick Y pin
// const int jsP = 8;  // Joystick button press pin

/* LED Matrix Defs */
const int ledDat = 12;
const int ledClk = 11;
const int ledCS = 10;
LedControl lc = LedControl(ledDat, ledClk, ledCS, 1);

/* Global Vars */
unsigned long prevMillis = 0;
unsigned long interval = 1000;

unsigned long prevMillis_blink = 0;
unsigned long interval_blink = 150;
int head_led_status = 1;

char in_byte;
Snake snake;
Food food;
bool do_grow = false;
bool game_over = false;

void seedRandom() {
    // Read from an unconnected analog pin to get a random seed
    randomSeed(analogRead(0));
}

void setup()
{
  // pinMode(jsP, INPUT_PULLUP); // Set the joystick button press pin to INPUT_PULLUP

  /*
    The MAX72XX is in power-saving mode on startup, we have to do a wakeup call.
  */
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
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

  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0) {
    in_byte = Serial.read();
    Snake::Direction curr_dir = snake.GetDir();
    Snake::Direction new_dir = curr_dir;
    switch (in_byte) {
      case 'u':
        if (curr_dir != Snake::Direction::down) {
          new_dir = Snake::Direction::up;
        }
        break;
      case 'd':
        if (curr_dir != Snake::Direction::up) {
          new_dir = Snake::Direction::down;
        }
        break;
      case 'l':
        if (curr_dir != Snake::Direction::right) {
          new_dir = Snake::Direction::left;
        }
        break;
      case 'r':
        if (curr_dir != Snake::Direction::left) {
          new_dir = Snake::Direction::right;
        }
        break;
      case 'g':
        do_grow = true;
        break;
      default:
        break;
    }
    snake.SetDir(new_dir);
  }

  Point head_pos = snake.GetChain().GetHead()->data;
  Point tail_pos = snake.GetChain().GetTail()->data;
  Point food_loc = food.GetCoords();
  
  unsigned long currMillis = millis();

  if (currMillis - prevMillis_blink >= interval_blink) {
    prevMillis_blink = currMillis;
    head_led_status = 1 - head_led_status;
    lc.setLed(0, head_pos.x, head_pos.y, head_led_status);
  }

  if (!game_over && currMillis - prevMillis >= interval) {
    prevMillis = currMillis;
    if (head_led_status == 0) {
      head_led_status = 1;
    }
    lc.setLed(0, head_pos.x, head_pos.y, head_led_status);

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

  if (game_over) {
    Serial.write("Game Over!");
    do {
      in_byte = Serial.read();
    } while (Serial.available() == 0);
  }
}
