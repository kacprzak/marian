#ifndef MATH_H
#define MATH_H

#include <cmath>

//int round(float n)
//{
//  return std::floor(n + 0.5f);
//}

template <typename T>
int sig(T val)
{
  return (val < T(0)) ? -1 : 1;
}

template <typename T>
int signum(T val)
{
  return (T(0) < val) - (val < T(0));
}

template <typename T>
class Vector2
{
 public:
  Vector2()
    : x(T(0)), y(T(0))
  {}

  Vector2(T ax, T ay)
    : x(ax), y(ay)
  {}

  Vector2(const Vector2<T>& v)
    : x(v.x), y(v.y)
  {}

  const Vector2<T>& operator=(const Vector2<T> &v)
  {	
    x = v.x;
    y = v.y;			
    return *this;
  }

  bool operator==(const Vector2<T> &v)
  {
    return (x == v.x && y == v.y);
  }
  
  bool operator!=(const Vector2<T> &v)
  {
    return (x != v.x || y != v.y);
  }
  
  bool isNull() const
  {
    return (x == 0 && y == 0) ? true : false;
  }
 
  const Vector2<T> operator+(const Vector2<T>& v) const
  {
    return Vector2<T>(x + v.x, y + v.y);
  }

  const Vector2<T>& operator+=(const Vector2<T>& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }
  
  const Vector2<T> operator-(const Vector2<T>& v) const
  {
    return Vector2<T>(x - v.x, y - v.y);
  }

  const Vector2<T>& operator-=(const Vector2<T>& v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  // Negation
  const Vector2<T> operator-(void) const
  {
    return Vector2<T>(-x, -y);
  }
		
  const Vector2<T>& operator*=(const T num)
  {
    x *= num;
    y *= num;										
    return *this;
  }
	
  const Vector2<T>& operator/=(const T num)
  {
    x /= num;
    y /= num;										
    return *this;
  }

  const Vector2<T> operator*(const T num) const
  {
    Vector2<T> temp(*this);			
    return temp *= num;
  }
  
  const Vector2<T> operator/(const T num) const
  {
    Vector2<T> temp(*this);			
    return temp /= num;
  }	

  // Dot product
  T operator* (const Vector2<T>& v) const
  {
    return x * v.x + y * v.y;
  }

  float length()
  {
    // Square root of dot product
    return std::sqrt(*this * *this);
  }
  
  T x, y;
};

//------------------------------------------------------------------------------

template <typename T>
class Rect
{
 public:
  Rect() {}

  Rect(const Rect<T>& r)
    : m_leftbottom(r.m_leftbottom)
    , m_size(r.m_size)
  {}

  Rect(T x, T y, T w, T h)
    : m_leftbottom(x, y)
    , m_size(w, h)
  {}

  Rect(Vector2<T> leftbottom, Vector2<T> size)
    : m_leftbottom(leftbottom)
    , m_size(size)
  {}

  const Vector2<T>& position() const { return m_leftbottom; }
  const Vector2<T>& size()     const { return m_size; }

  // Corners
  Vector2<T> leftbottom() const { return m_leftbottom; }

  Vector2<T> rightbottom() const
  {
    return Vector2<T>(right(), bottom());
  }

  Vector2<T> lefttop() const
  {
    return Vector2<T>(left(), top());
  }

  Vector2<T> righttop() const
  {
    return m_leftbottom + m_size;
  }

  bool isNull() const { return m_size.isNull(); }

  T left()   const { return m_leftbottom.x; }
  T right()  const { return m_leftbottom.x + m_size.x; }
  T top()    const { return m_leftbottom.y + m_size.y; }
  T bottom() const { return m_leftbottom.y; }

  void move(T x, T y)
  {
    m_leftbottom.x += x;
    m_leftbottom.y += y;
  }

  void move(const Vector2<T>& v)
  {
    m_leftbottom += v;
  }

  void moveTo(T x, T y)
  {
    m_leftbottom.x = x;
    m_leftbottom.y = y;
  }

  void moveTo(const Vector2<T>& v)
  {
    m_leftbottom = v;
  }

  const Vector2<T> center() const
  {
    T x = left() + m_size.x / 2;
    T y = bottom() + m_size.y / 2;
    return Vector2<T>(x, y);
  }

  // Very simple true/false collision detection
  bool intersects(const Rect<T>& other) const
  {
    return !(other.left()      > right()
             || other.right()  < left()
             || other.bottom() > top()
             || other.top()    < bottom());
  }

  /**
   *  _______
   * |       |
   * |    __ |__
   * |   |\  |  |
   * |   | \ |  |
   * |___|__\|  |
   *     |      |
   *     |______|
   */
  const Vector2<T> penetrationVector(const Rect<T>& other) const
  {
    Vector2<T> pv; // null vector

    // Distance between centers
    Vector2<T> dist = center() - other.center();
    T ad_x = std::abs(dist.x);
    T ad_y = std::abs(dist.y);   

    // Combined width
    T cw = size().x / 2 + other.size().x / 2;
    T ch = size().y / 2 + other.size().y / 2;

    // Early exit
    if (ad_x > cw || ad_y > ch)
      return pv;

    pv.x = -(ad_x - cw);
    pv.y = -(ad_y - ch);

    pv.x *= sig<T>(dist.x);
    pv.y *= sig<T>(dist.y);
    return pv;
  }

  // Discard corner collisions
#define CORNER_EPSILON 5.0f

  const Vector2<T> escapeVector(const Rect<T>& other) const
  {
    Vector2<T> pv = penetrationVector(other);

    // Corner collision
    if (std::abs(pv.x) < CORNER_EPSILON
        && std::abs(pv.y) < CORNER_EPSILON) {
      return Vector2<T>();
    }

    if (std::abs(pv.x) > std::abs(pv.y))
      pv.x = T(0);
    else if (std::abs(pv.y) > std::abs(pv.x))
      pv.y = T(0);

    return pv;
  }

 private:
  Vector2<T> m_leftbottom;
  Vector2<T> m_size;
};

//------------------------------------------------------------------------------

#endif
