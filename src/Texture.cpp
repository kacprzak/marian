/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Texture.h"

#include "Util.h"

Texture::Texture()
    : m_textureId(0)
    , m_w(0)
    , m_h(0)
{
    //
}

Texture::~Texture()
{
    //
}

bool Texture::loadFromFile(const std::string& filename)
{
    m_textureId = load_texture(filename.c_str(), &m_w, &m_h);
    return m_textureId == 0 ? false : true;
}

void Texture::release()
{
    glDeleteTextures(1, &m_textureId);  
}
