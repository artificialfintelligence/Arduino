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

  Snake();
  Snake(Point p, Direction dir);

  void Move();
  void Grow();

  Direction GetDir() const;
  void SetDir(Direction dir);
  Point GetP() const;
  void SetP(Point p);

private:
  Point XformCoords(Point p) const;

  Direction dir_;
  Point p_;
};

#endif // SNAKE_H
