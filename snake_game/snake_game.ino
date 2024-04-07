#include "LedControl.h"
#include "snake.h"
#include "point.h"

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
unsigned long previousMillis = 0;
unsigned long interval = 500;
char in_byte;
Snake snake;
bool do_grow = false;

void setup()
{
  // pinMode(jsP, INPUT_PULLUP); // Set the joystick button press pin to INPUT_PULLUP

  /*
    The MAX72XX is in power-saving mode on startup, we have to do a wakeup call.
  */
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  snake = Snake();
  Point init_p = snake.GetChain().GetHead()->data;
  lc.setLed(0, init_p.x, init_p.y, true);

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

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Point old_tail_pos = snake.GetChain().GetTail()->data;
    snake.Update(do_grow);
    Point new_head_pos = snake.GetChain().GetHead()->data;
    lc.setLed(0, new_head_pos.x, new_head_pos.y, 1);
    if (!do_grow && snake.GetDir() != Snake::Direction::none) {
      lc.setLed(0, old_tail_pos.x, old_tail_pos.y, 0);
    }
    do_grow = false;
  }
}
