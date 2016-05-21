/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <GL/gl.h>
#include <string>

#include <boost/noncopyable.hpp>

// Forward declaration
struct SDL_Surface;

namespace gfx {

/*!
 * \brief Class for holding an image in graphics card memory.
 */
class Texture : private boost::noncopyable
{
 public:
    Texture();
    ~Texture();

    GLuint textureId() const { return m_textureId; }
    int w() const { return m_w; }
    int h() const { return m_h; }

    void loadFromFile(const std::string& filename);
    void loadFromSDL(SDL_Surface *surface);

 private:
    GLuint m_textureId;
    int m_w;
    int m_h;
};

} // namespace gfx

#endif
