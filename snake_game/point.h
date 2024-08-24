#ifndef POINT_H
#define POINT_H

struct Point {
  int x;
  int y;

  // Override the Equality Operator
  bool operator==(const Point& other) const {
    return (x == other.x && y == other.y);
  }

  // Override the Inequality Operator
  bool operator!=(const Point& other) const {
    return !(*this == other);
  }
};

#endif // POINT_H
