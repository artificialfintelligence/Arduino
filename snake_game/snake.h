#ifndef SNAKE_H
#define SNAKE_H

class Snake {
public:
  enum class Direction {
    up,
    down,
    left,
    right
  };
  Direction dir_;

  Snake(int x, int y, Direction dir);
  
  void Move();

  int getX() const;
  int getY() const;
  
private:
  int x_;
  int y_;
};

#endif // SNAKE_H
