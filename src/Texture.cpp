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

//------------------------------------------------------------------------------

void Texture::calculateTextureCoords(GLfloat texCoords[8], int texWidth, int texHeight,
                                     int x0, int y0, int x1, int y1)
{
    if (x1 == 0) x1 = texWidth;
    if (y1 == 0) y1 = texHeight;

    GLfloat s0 = x0 / (float)texWidth;
    GLfloat t0 = y0 / (float)texHeight;
    GLfloat s1 = x1 / (float)texWidth;
    GLfloat t1 = y1 / (float)texHeight;

    texCoords[0] = s0; texCoords[1] = t0;
    texCoords[2] = s1; texCoords[3] = t0;
    texCoords[4] = s1; texCoords[5] = t1;
    texCoords[6] = s0; texCoords[7] = t1;
}

//------------------------------------------------------------------------------

void Texture::flipVerticallyTextureCoords(GLfloat texCoords[8])
{
    texCoords[0] = texCoords[2];
    texCoords[2] = texCoords[6];
    texCoords[4] = texCoords[2];
    texCoords[6] = texCoords[0];
}

//------------------------------------------------------------------------------

void Texture::flipHorizontallyTextureCoords(GLfloat texCoords[8])
{
    texCoords[1] = texCoords[5];
    texCoords[5] = texCoords[3];
    texCoords[7] = texCoords[3];
    texCoords[3] = texCoords[1];
}
