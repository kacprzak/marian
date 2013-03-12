/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef IMAGE_H
#define IMAGE_H

#include "Texture.h"
#include <string>
#include <sstream>
#include <ostream>
#include "Util.h"

/**
 * An image. Represents whole or part of a texture.
 */
class Image {
 public:
    Image(const Texture *texture)
        : m_texture(texture)
        , m_pixelWidth(m_texture->w())
        , m_pixelHeight(m_texture->h())
        , m_width(1.0f)
        , m_height(1.0f)
    {
        calculateTextureCoords(m_texCoords, texture->w(), texture->h());
    }

    Image(const Texture *texture, int x0, int y0, int x1, int y1)
        : m_texture(texture)
        , m_pixelWidth(x1 - x0)
        , m_pixelHeight(y1 - y0)
        , m_width(1.0f)
        , m_height(1.0f)
    {
        calculateTextureCoords(m_texCoords, texture->w(), texture->h(),
                               x0, y0, x1, y1);
    }

    int pixelWidth() const  { return m_pixelWidth; }
    int pixelHeight() const { return m_pixelHeight; }

    float width() const  { return m_width;  }
    float height() const { return m_height; }
    void setWidth(float w)  { m_width = w;  }
    void setHeight(float h) { m_height = h; }

    void scale(float factor)
    {
        m_width *= factor;
        m_height *= factor;
    }

    const Texture *texture() const { return m_texture; }
    const GLfloat *getTextureCoords() const { return m_texCoords; }
    
    std::string toString() const
    {
        std::stringstream ss;
        ss << "Image: {" << this
           << ", pixelSize: {" << m_pixelWidth << ", " << m_pixelHeight << "}"
           << ", texId: " << m_texture->textureId() << "}";
        return ss.str();
    }

 private:
    const Texture *m_texture;
    int            m_pixelWidth;
    int            m_pixelHeight;
    float          m_width;
    float          m_height;
    GLfloat        m_texCoords[8];
};


inline std::ostream& operator<<(std::ostream& os, const Image& image)
{
    os << image.toString();
    return os;
}

#endif
