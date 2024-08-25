#include "food.h"
#include "point.h"
#include "invdblll.hpp"
#include <Arduino.h>

extern void seedRandom();

// Default constructor (random location)
Food::Food() {
    seedRandom();
    coords_ = XformCoords(Point{static_cast<int>(random(8)), static_cast<int>(random(8))});
}

void Food::Spawn(const InvertedDoubleLinkedList<Point, 64>& excl_coords_list) {
  Point candidate;
  bool is_valid_loc;
  do {
    is_valid_loc = true;
    candidate = Point{static_cast<int>(random(8)), static_cast<int>(random(8))};
    Node<Point>* node = excl_coords_list.GetHead();
    while (node != nullptr) {
      if (node->data == candidate) {
        is_valid_loc = false;
        break;
      }
      node = node->prev;
    }
  } while (!is_valid_loc);
  coords_ = candidate;
}

Point Food::GetCoords() const {
  return coords_;
}

Point Food::XformCoords(Point p) const {
  return {7 - p.y, p.x};
}
