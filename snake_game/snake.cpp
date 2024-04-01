#include "snake.h"

using Direction = Snake::Direction;

Snake::Snake(int x, int y, Direction dir) : x_(x), y_(y), dir_(dir) {}

void Snake::Move() {
  switch (dir_) {
    case Direction::up:
      y_ = --y_ % 8;
      break;
    case Direction::down:
      y_ = ++y_ % 8;
      break;
    case Direction::left:
      x_ = --x_ % 8;
      break;
    case Direction::right:
      x_ = ++x_ % 8;
      break;
    default:
      break;
  }
}

int Snake::getX() const {
  return x_;
}

int Snake::getY() const {
  return y_;
}

