#ifndef SPRITE_H
#define SPRITE_H

#include "Math.h"
#include "Texture.h"

class Sprite {
 public:
  Sprite(const Texture *texture)
    : m_position(0.0f, 0.0f)
    , m_texture(texture)
    , m_textureRect(0, 0, m_texture->w(), m_texture->h())
    , m_size(m_textureRect.size())
  {
    //
  }

 Sprite(const Texture *texture, Rect<int> textureRect)
    : m_position(0.0f, 0.0f)
    , m_texture(texture)
    , m_textureRect(textureRect)
    , m_size(m_textureRect.size())
  {
    //
  }

  void setPosition(float x, float y)
  {
    m_position.x = x;
    m_position.y = y;
  }

  int width() const
  {
    return m_size.x;
  }

  int height() const
  {
    return m_size.y;
  }

  const Vector2<float> position() const
  {
    return m_position;
  }

  const Texture *texture() const
  {
    return m_texture;
  }

  void getTextureCoords(float texCoords[]) const;

 private:
  Vector2<float> m_position;
  const Texture *m_texture;
  Rect<int>      m_textureRect;
  Vector2<int>   m_size;
};

#endif
