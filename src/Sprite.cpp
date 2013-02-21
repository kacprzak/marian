#include "Sprite.h"

#include <cmath>

const GLfloat *Sprite::getTextureCoords() const
{
  return m_texCoords;
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
