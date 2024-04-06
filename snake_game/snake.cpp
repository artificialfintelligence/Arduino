#include "snake.h"
#include "point.h"

using Direction = Snake::Direction;

// Default constructor
Snake::Snake() : p_(XformCoords({0, 0})), dir_(Direction::right) {}

Snake::Snake(Point p, Direction dir) : p_(XformCoords(p)), dir_(dir) {}

void Snake::Move() {
  Point curr_p = p_;
  int x = curr_p.x;
  int y = curr_p.y;
  switch (dir_) {
    case Direction::left:   // left is actually up:
      y = --y % 8;
      if (y < 0) {
        y += 8;
      }
      break;
    case Direction::right:  // right is actually down
      y = ++y % 8;
      break;
    case Direction::up:     // up is actually left
      x = --x % 8;
      if (x < 0) {
        x += 8;
      }
      break;
    case Direction::down:   //down is actually right
      x = ++x % 8;
      break;
    default:
      // Don't move - should only happen when game starts
      break;
  }
  SetP({x, y});
}

void Snake::Grow() {

}

Direction Snake::GetDir() const {
  return dir_;
}

void Snake::SetDir(Direction dir) {
  dir_ = dir;
}

Point Snake::GetP() const {
  return p_;
}

void Snake::SetP(Point p) {
  p_ = p;
}

Point Snake::XformCoords(Point p) const {
  return {7 - p.y, p.x};
}
