#include "Sprite.h"

#include <cmath>

void Sprite::getTextureCoords(GLfloat texCoords[]) const
{
  const Rect<int>& rect = m_textureRect;

  float w = static_cast<float>(m_texture->w());
  float h = static_cast<float>(m_texture->h());

  if (!rect.isNull()) {
    GLfloat u0 = rect.left()   / w;
    GLfloat v0 = rect.bottom() / h;
    GLfloat u1 = rect.right()  / w;
    GLfloat v1 = rect.top()    / h;

    // Weird but this repairs glitches while rendering tiles on mesa libs
    v1 -= 0.0001f;

    texCoords[0] = u0; texCoords[1] = v0;
    texCoords[2] = u1; texCoords[3] = v0;
    texCoords[4] = u1; texCoords[5] = v1;
    texCoords[6] = u0; texCoords[7] = v1;      
  } else {
    texCoords[0] = 0.0f; texCoords[1] = 0.0f;
    texCoords[2] = 1.0f; texCoords[3] = 0.0f;
    texCoords[4] = 1.0f; texCoords[5] = 1.0f;
    texCoords[6] = 0.0f; texCoords[7] = 1.0f;
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
