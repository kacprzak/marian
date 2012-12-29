#ifndef MATH_H
#define MATH_H

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
  Rect(T x, T y, T w, T h)
    : m_leftbottom(x, y)
    , m_size(w, h)
    {}

  Rect(Vector2<T> topleft, Vector2<T> size)
    : m_leftbottom(topleft)
    , m_size(size)
    {}

  const Vector2<T> leftbottom() const { return m_leftbottom; }
  const Vector2<T> size()    const { return m_size; }

  T left()   const { return m_leftbottom.x; }
  T right()  const { return m_leftbottom.x + m_size.x; }
  T top()    const { return m_leftbottom.y + m_size.y; }
  T bottom() const { return m_leftbottom.y; }

  const Vector2<T> center() const
  {
    T x = left() + m_size.x / 2;
    T y = bottom() + m_size.y / 2;
    return Vector2<T>(x, y);
  }

  bool intersects(const Rect<T>& other)
  {
    return !(other.left()      > right()
             || other.right()  < left()
             || other.bottom() > top()
             || other.top()    < bottom());
  }

 private:
  Vector2<T> m_leftbottom;
  Vector2<T> m_size;
};

#endif
