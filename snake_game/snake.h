#ifndef SNAKE_H
#define SNAKE_H

#include "point.h"

class Snake {
public:
  enum class Direction {
    up,
    down,
    left,
    right
  };
  Direction dir_;

  Snake(Point p, Direction dir);

  void Move();

  int getX() const;
  int getY() const;

private:
  int x_;
  int y_;
};

#endif // SNAKE_H
