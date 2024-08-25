#include "food.h"
#include "point.h"
#include "invdblll.hpp"
#include <Arduino.h>

extern void seedRandom();

// Default constructor (random location)
Food::Food() {
    seedRandom();
    coords_ = XformCoords(Point{random(8), random(8)});
}

// void Food::Spawn(InvertedDoubleLinkedList<Point, 64> excl_coords) {
//   // TODO
// }

Point Food::GetCoords() const {
  return coords_;
}

Point Food::XformCoords(Point p) const {
  return {7 - p.y, p.x};
}
