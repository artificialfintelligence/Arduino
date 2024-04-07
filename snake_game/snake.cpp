#include "snake.h"
#include "point.h"
#include "invdblll.hpp"

using Direction = Snake::Direction;

// Default constructor
Snake::Snake() : dir_(Direction::none), length_(1) {
  chain_.HeadAppend(XformCoords({0, 0}));
}

void Snake::Update(bool do_grow) {
  // Point curr_p = p_;
  Node<Point>* head = chain_.GetHead();
  if (head != nullptr) {
    Point head_coords = head->data;
    int x = head_coords.x;
    int y = head_coords.y;
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
    chain_.HeadAppend({x, y});

    if (do_grow) {
      length_++;
    }
    else {
      chain_.TailDelete();
    }
  }
}

Direction Snake::GetDir() const {
  return dir_;
}

void Snake::SetDir(Direction dir) {
  dir_ = dir;
}

int Snake::GetLength() const {
  return length_;
}

InvertedDoubleLinkedList<Point, 64> Snake::GetChain() const {
  return chain_;
}

Point Snake::XformCoords(Point p) const {
  return {7 - p.y, p.x};
}
