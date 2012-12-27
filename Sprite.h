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

  void getTextureCoords(float texCoords[]) const
  {
    const Rect<int>& rect = m_textureRect;
    float w = static_cast<float>(m_texture->w());
    float h = static_cast<float>(m_texture->h());

    if (rect.size().x > 0 && rect.size().y > 0) {
      texCoords[0] = rect.left()  / w; texCoords[1] = rect.bottom() / h;
      texCoords[2] = rect.right() / w; texCoords[3] = rect.bottom() / h;
      texCoords[4] = rect.right() / w; texCoords[5] = rect.top()    / h;
      texCoords[6] = rect.left()  / w; texCoords[7] = rect.top()    / h;      
    } else {
      texCoords[0] = 0.0f; texCoords[1] = 0.0f;
      texCoords[2] = 1.0f; texCoords[3] = 0.0f;
      texCoords[4] = 1.0f; texCoords[5] = 1.0f;
      texCoords[6] = 0.0f; texCoords[7] = 1.0f;
    }
  }

 private:
  Vector2<float> m_position;
  const Texture *m_texture;
  Rect<int>      m_textureRect;
  Vector2<int>   m_size;
};

#endif
