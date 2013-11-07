/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <GL/gl.h>
#include <string>

/** Class for holding an image in graphics card memory */
class Texture
{
 public:
    Texture();
    ~Texture();

    GLuint textureId() const { return m_textureId; }
    int w() const { return m_w; }
    int h() const { return m_h; }

    bool loadFromFile(const std::string& filename);

 private:
    GLuint m_textureId;
    int m_w;
    int m_h;
};

/* Texture coords utils. */

/** Single texture coord. S and T have bounds [0, 1] */
struct TexCoord
{
    GLfloat s, t;
};

template <std::size_t size>
struct TexCoords
{
    TexCoord coords[size];
};

// Static functions
TexCoords<4> calculateTextureCoords(int texWidth, int texHeight,
                                    int x0 = 0, int y0 = 0, int x1 = 0, int y1 = 0);
TexCoords<4> flipVerticallyTextureCoords(const TexCoords<4>& texCoords);
TexCoords<4> flipHorizontallyTextureCoords(const TexCoords<4>& texCoords);

#endif
