#include "Sprite.h"

void Sprite::getTextureCoords(GLfloat texCoords[]) const
{
  const Rect<int>& rect = m_textureRect;

  float w = static_cast<float>(m_texture->w());
  float h = static_cast<float>(m_texture->h());
  
  // Weird but this repairs glitches while rendering tiles on mesa libs
  //h += 32.0f / h;

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

std::string Sprite::toString() const
{
  std::stringstream ss;
  ss << "Sprite: {" << this << ", pos: {" << m_position.x
     << ", " << m_position.y << "}, size: {"
     << m_size.x << ", " << m_size.y << "}, texId: "
     << m_texture->textureId() << "}";
  return ss.str();
}
