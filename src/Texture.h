/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <GL/gl.h>
#include <string>

class Texture {
 public:
    Texture();
    ~Texture();

    GLuint textureId() const { return m_textureId; }
    int w() const { return m_w; }
    int h() const { return m_h; }

    bool loadFromFile(const std::string& filename);
    void release();

 private:
    GLuint m_textureId;
    int m_w;
    int m_h;
};

#endif
