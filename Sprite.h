#ifndef SPRITE_H
#define SPRITE_H

#include "Math.h"
#include "Texture.h"

class Sprite {
 public:
  Sprite(const Texture *texture)
    : m_position(0.0f, 0.0f)
    , m_texture(texture)
  {
    //
  }

  void setPosition(float x, float y)
  {
    m_position.x = x;
    m_position.y = y;
  }

  const Vector2<float> position() const
  {
    return m_position;
  }

  const Texture *texture() const
  {
    return m_texture;
  }

 private:
  Vector2<float> m_position;
  const Texture *m_texture;
};

#endif
