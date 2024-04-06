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

void setup()
{
  // pinMode(jsP, INPUT_PULLUP); // Set the joystick button press pin to INPUT_PULLUP

  /*
    The MAX72XX is in power-saving mode on startup, we have to do a wakeup call.
  */
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  snake = Snake({0, 0}, Snake::Direction::right);
  lc.setLed(0, snake.GetP().x, snake.GetP().y, true);

  Serial.begin(9600);
}

void loop()
{
  Snake::Direction new_dir;
  if (Serial.available() > 0) {
    in_byte = Serial.read();
    if (in_byte == 'u') {
      new_dir = Snake::Direction::up;
    }
    else if (in_byte == 'd') {
      new_dir = Snake::Direction::down;
    }
    else if (in_byte == 'l') {
      new_dir = Snake::Direction::left;
    }
    else if (in_byte == 'r') {
      new_dir = Snake::Direction::right;
    }
    else if (in_byte == 'g') {
      snake.Grow();
    }
  }
  snake.SetDir(new_dir);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Point p = snake.GetP();
    lc.setLed(0, p.x, p.y, 0);
    snake.Move();
    p = snake.GetP();
    lc.setLed(0, p.x, p.y, 1);
  }
}
