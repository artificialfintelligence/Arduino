#ifndef FOOD_H
#define FOOD_H

#include "point.h"
#include "invdblll.hpp"
#include <Arduino.h>

class Food {
public:
  Food();

  void Spawn(const InvertedDoubleLinkedList<Point, 64>& excl_coords_list);

  Point GetCoords() const;
  // void SetCoords(Point p);

private:
  Point XformCoords(Point p) const;
  Point coords_;
};

#endif // FOOD_H
