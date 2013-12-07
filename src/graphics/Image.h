/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef IMAGE_H
#define IMAGE_H

#include "Texture.h"
#include "TexCoords.h"

#include <string>
#include <sstream>
#include <ostream>

namespace gfx {

/*!
 * \brief An image. Represents whole or part of a texture.
 *
 * It's a light object. Can be passed to fuctions as copy. 
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
        m_texCoords[0] = TexCoord(0.0f, 0.0f);
        m_texCoords[1] = TexCoord(1.0f, 0.0f);
        m_texCoords[2] = TexCoord(1.0f, 1.0f);
        m_texCoords[3] = TexCoord(0.0f, 1.0f);
    }

    Image(const Texture *texture, const Rect<int>& tileCoords)
        : m_texture(texture)
        , m_pixelWidth(tileCoords.right - tileCoords.left)
        , m_pixelHeight(tileCoords.top - tileCoords.bottom)
        , m_width(1.0f)
        , m_height(1.0f)
    {
        m_texCoords = calculateTexCoords(texture->w(), texture->h(), tileCoords);
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

    void flipVertically()
    {
        m_texCoords = flipVerticallyTexCoords(m_texCoords);
    }
    
    void flipHorizontally()
    {
        m_texCoords = flipHorizontallyTexCoords(m_texCoords);
    }

    const Texture *texture() const { return m_texture; }
    const float *getTextureCoords() const { return reinterpret_cast<const float *>(&m_texCoords); }
    
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
    TexCoords<4>   m_texCoords;
};

} // namespace gfx

inline std::ostream& operator<<(std::ostream& os, const gfx::Image& image)
{
    os << image.toString();
    return os;
}

#endif
