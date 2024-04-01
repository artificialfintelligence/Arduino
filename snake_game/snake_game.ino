#include "LedControl.h"
#include "snake.h"

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
Snake::Direction curr_dir = Snake::Direction::right;
Snake snake(0, 0, curr_dir);

void setup()
{
  // pinMode(jsP, INPUT_PULLUP); // Set the joystick button press pin to INPUT_PULLUP

  /*
    The MAX72XX is in power-saving mode on startup,
    we have to do a wakeup call
  */
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  lc.setLed(0, 0, 0, true);

  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0) {
    in_byte = Serial.read();
    if (in_byte == 'u') { 
      curr_dir = Snake::Direction::up;
    }
    else if (in_byte == 'd') { 
      curr_dir = Snake::Direction::down;
    }
    else if (in_byte == 'l') { 
      curr_dir = Snake::Direction::left;
    }
    else if (in_byte == 'r') { 
      curr_dir = Snake::Direction::right;
    }
  }
  snake.dir_ = curr_dir;
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    lc.setLed(0, snake.getX(), snake.getY(), 0);
    snake.Move();
    lc.setLed(0, snake.getX(), snake.getY(), 1);
  }
}