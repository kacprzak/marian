/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef SPRITE_H
#define SPRITE_H

#include "Math.h"
#include "Texture.h"
#include <string>
#include <sstream>
#include <ostream>
#include "Util.h"

/**
 * An image. Represents whole or part of a texture.
 */
class Sprite {
 public:
    Sprite(const Texture *texture)
        : m_texture(texture)
        , m_size(m_texture->w(), m_texture->h())
    {
        calculateTextureCoords(texture->w(), texture->h(), Rect<int>(), m_texCoords);
    }

    Sprite(const Texture *texture, Rect<int> textureRect)
        : m_texture(texture)
        , m_size(textureRect.size())
    {
        calculateTextureCoords(texture->w(), texture->h(), textureRect, m_texCoords);
    }

    int pixelWidth() const  { return m_size.x; }
    int pixelHeight() const { return m_size.y; }

    const Texture *texture() const { return m_texture; }
    const GLfloat *getTextureCoords() const { return m_texCoords; }
    
    std::string toString() const
    {
        std::stringstream ss;
        ss << "Sprite: {" << this
           << ", size: {" << m_size.x << ", " << m_size.y << "}"
           << ", texId: " << m_texture->textureId() << "}";
        return ss.str();
    }

 private:
    const Texture *m_texture;
    Vector2<int>   m_size; //< Pixel size
    GLfloat        m_texCoords[8];
};


inline std::ostream& operator<<(std::ostream& os, const Sprite& sprite)
{
    os << sprite.toString();
    return os;
}

#endif
