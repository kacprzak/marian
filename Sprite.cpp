#include "Sprite.h"

#include <cmath>

const GLfloat *Sprite::getTextureCoords() const
{
  return m_texCoords;
}

void Sprite::calculateTextureCoords(const Rect<int>& rect)
{
  float w = static_cast<float>(m_texture->w());
  float h = static_cast<float>(m_texture->h());

  if (!rect.isNull()) {
    GLfloat s0 = rect.left()   / w;
    GLfloat t0 = rect.bottom() / h;
    GLfloat s1 = rect.right()  / w;
    GLfloat t1 = rect.top()    / h;

    // Weird but this repairs glitches while rendering tiles on mesa libs
    //t1 -= 0.0001f;

    m_texCoords[0] = s0; m_texCoords[1] = t0;
    m_texCoords[2] = s1; m_texCoords[3] = t0;
    m_texCoords[4] = s1; m_texCoords[5] = t1;
    m_texCoords[6] = s0; m_texCoords[7] = t1;      
  } else {
    m_texCoords[0] = 0.0f; m_texCoords[1] = 0.0f;
    m_texCoords[2] = 1.0f; m_texCoords[3] = 0.0f;
    m_texCoords[4] = 1.0f; m_texCoords[5] = 1.0f;
    m_texCoords[6] = 0.0f; m_texCoords[7] = 1.0f;
  }
}

std::string Sprite::toString() const
{
  std::stringstream ss;
  ss << "Sprite: {" << this << ", pos: {" << m_position.x
     << ", " << m_position.y << "}, size: {"
     << m_size.x << ", " << m_size.y << "}, texId: "
     << m_texture->textureId() << "}";
  return ss.str();
}
