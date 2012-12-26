#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>
#include <string>
#include <boost/utility.hpp>

class Texture : boost::noncopyable {
 public:
  Texture();
  ~Texture();

  GLuint textureId() { return m_textureId; }
  int w() { return m_w; }
  int h() { return m_h; }

  bool loadFromFile(const std::string& filename);

 private:
  GLuint m_textureId;
  int m_w;
  int m_h;
};

#endif
