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

  snake = Snake();
  Point init_p = snake.GetChain().GetHead()->data;
  lc.setLed(0, init_p.x, init_p.y, true);

  Serial.begin(9600);
}

void loop()
{
  Snake::Direction new_dir = snake.GetDir();
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
    lc.clearDisplay(0);
    snake.Move();
    Node<Point>* p = snake.GetChain().GetHead();
    while (p != nullptr) {
      lc.setLed(0, p->data.x, p->data.y, 1);
      p = p->next;
    }
  }
}
