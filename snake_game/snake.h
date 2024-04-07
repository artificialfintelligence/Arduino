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
    right,
    none
  };

  Snake();

  void Update(bool do_grow);

  Direction GetDir() const;
  void SetDir(Direction dir);

  int GetLength() const;

  InvertedDoubleLinkedList<Point, 64> GetChain() const;

private:
  Point XformCoords(Point p) const;
  Direction dir_;
  int length_;
  InvertedDoubleLinkedList<Point, 64> chain_;
};

#endif // SNAKE_H
