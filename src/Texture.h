/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <GL/gl.h>
#include <string>

/** Class for holding an image in graphics card memory */
class Texture {
 public:
    Texture();
    ~Texture();

    GLuint textureId() const { return m_textureId; }
    int w() const { return m_w; }
    int h() const { return m_h; }

    bool loadFromFile(const std::string& filename);

    // Static functions
    static void calculateTextureCoords(GLfloat texCoords[8],
                                       int texWidth, int texHeight,
                                       int x0 = 0, int y0 = 0, int x1 = 0, int y1 = 0);

    static void flipVerticallyTextureCoords(GLfloat texCoords[8]);

    static void flipHorizontallyTextureCoords(GLfloat texCoords[8]);

 private:
    GLuint m_textureId;
    int m_w;
    int m_h;
};

#endif
