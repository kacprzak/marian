#ifndef VECTOR2_H
#define VECTOR2_H

template <typename T>
class Vector2 {
 public:
  Vector2(T ax, T ay)
    : x(ax), y(ay) {}

  T x, y;
};

template <typename T>
class Rect {
 public:
  Rect(T ax, T ay, T aw, T ah)
    : topleft(ax, ay), size(aw, ah) {}

  Rect(Vector2<T> atopleft, Vector2<T> asize)
    : topleft(atopleft), size(asize) {}

  Vector2<T> topleft;
  Vector2<T> size;
};

#endif
