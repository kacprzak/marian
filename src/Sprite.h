#ifndef SPRITE_H
#define SPRITE_H

#include "Math.h"
#include "Texture.h"
#include <string>
#include <sstream>
#include <ostream>
#include "Util.h"

class Sprite {
 public:
  Sprite(const Texture *texture)
    : m_position(0.0f, 0.0f)
    , m_texture(texture)
    , m_size(m_texture->w(), m_texture->h())
  {
    calculateTextureCoords(texture->w(), texture->h(), Rect<int>(), m_texCoords);
  }

 Sprite(const Texture *texture, Rect<int> textureRect)
    : m_position(0.0f, 0.0f)
    , m_texture(texture)
    , m_size(textureRect.size())
  {
    calculateTextureCoords(texture->w(), texture->h(), textureRect, m_texCoords);
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

  int width() const
  {
    return m_size.x;
  }

  int height() const
  {
    return m_size.y;
  }

  const Texture *texture() const
  {
    return m_texture;
  }

  const GLfloat *getTextureCoords() const;
  std::string toString() const;

 private:
  Vector2<float> m_position;
  const Texture *m_texture;
  Vector2<int>   m_size;
  GLfloat        m_texCoords[8];
};


inline std::ostream& operator<<(std::ostream& os, const Sprite& sprite)
{
  os << sprite.toString();
  return os;
}

#endif
