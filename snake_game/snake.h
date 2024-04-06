#ifndef SNAKE_H
#define SNAKE_H

#include "point.h"
#include "invdblll.hpp"

class Snake {
public:
  enum class Direction {
    up,
    down,
    left,
    right
  };

  Snake();

  void Move();
  void Grow();

  Direction GetDir() const;
  void SetDir(Direction dir);

  InvertedDoubleLinkedList<Point, 64> GetChain() const;

private:
  Point XformCoords(Point p) const;

  Direction dir_;
  bool grow_flag_;
  InvertedDoubleLinkedList<Point, 64> chain_;
};

#endif // SNAKE_H
