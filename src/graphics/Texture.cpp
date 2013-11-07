/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Texture.h"

#include "Util.h"

Texture::Texture()
    : m_textureId(0)
    , m_w(0)
    , m_h(0)
{}

//------------------------------------------------------------------------------

Texture::~Texture()
{
    glDeleteTextures(1, &m_textureId);
}

//------------------------------------------------------------------------------

bool Texture::loadFromFile(const std::string& filename)
{
    m_textureId = load_texture(filename.c_str(), &m_w, &m_h);
    return m_textureId == 0 ? false : true;
}

//==============================================================================

TexCoords<4> calculateTextureCoords(int texWidth, int texHeight,
                                    int x0, int y0, int x1, int y1)
{   
    TexCoords<4> cs;

    if (x1 == 0) x1 = texWidth;
    if (y1 == 0) y1 = texHeight;

    // Left bottom
    cs.coords[0].s = x0 / (float)texWidth;
    cs.coords[0].t = y0 / (float)texHeight;

    // Top right
    cs.coords[2].s = x1 / (float)texWidth;
    cs.coords[2].t = y1 / (float)texHeight;

    // Right bottom
    cs.coords[1].s = cs.coords[2].s;
    cs.coords[1].t = cs.coords[0].t;

    // Left top
    cs.coords[3].s = cs.coords[0].s;
    cs.coords[3].t = cs.coords[2].t;

    return cs;
}

//------------------------------------------------------------------------------

TexCoords<4> flipVerticallyTextureCoords(const TexCoords<4>& texCoords)
{   
    TexCoords<4> cs;

    cs.coords[0] = texCoords.coords[1];
    cs.coords[1] = texCoords.coords[0];
    cs.coords[2] = texCoords.coords[3];
    cs.coords[3] = texCoords.coords[2];

    return cs;
}

//------------------------------------------------------------------------------

TexCoords<4> flipHorizontallyTextureCoords(const TexCoords<4>& texCoords)
{
    TexCoords<4> cs;

    cs.coords[0] = texCoords.coords[3];
    cs.coords[1] = texCoords.coords[2];
    cs.coords[2] = texCoords.coords[1];
    cs.coords[3] = texCoords.coords[0];

    return cs;
}
