#include "snake.h"
#include "point.h"

using Direction = Snake::Direction;

Snake::Snake(Point p, Direction dir) : dir_(dir) {
  x_ = 7 - p.y;
  y_ = p.x;
}

void Snake::Move() {
  switch (dir_) {
    case Direction::left:   // left is actually up:
      y_ = --y_ % 8;
      if (y_ < 0) {
        y_ += 8;
      }
      break;
    case Direction::right:  // right is actually down
      y_ = ++y_ % 8;
      break;
    case Direction::up:     // up is actually left
      x_ = --x_ % 8;
      if (x_ < 0) {
        x_ += 8;
      }
      break;
    case Direction::down:   //down is actually right
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
