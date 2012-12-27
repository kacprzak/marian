#ifndef ENGINE_H
#define ENGINE_h

#include <GL/gl.h>
#include <GL/glu.h>

#include <string>
#include <exception>
#include "Playable.h"
#include "Sprite.h"

class SdlError : public std::exception
{
 public:
  SdlError(const std::string& msg, char *sdlError)
    : m_msg(msg)
    , m_sdlError(sdlError)
  {
    //
  }

  const char* what() const noexcept
  {
    std::string fullMsg = m_msg + ": " + m_sdlError;
    return fullMsg.c_str();
  }

 private:
  const std::string m_msg;
  char *m_sdlError;
};

//------------------------------------------------------------------------------

class Engine
{
 public:
  Engine(const std::string& title, int screenWidth, int screenHeight);
  ~Engine();
  
  void mainLoop(Playable *game);

  void drawQuad(float x, float y, float w, float h);
  void drawQuad(float x, float y, float w, float h, float texCoords[]);
  void drawQuad(float x, float y, float w, float h, GLuint textureId, float texCoords[]);
  void drawSprite(const Sprite& sprite);

 private:
  void initializeSDL();
  void initializeOpenGL();

  bool processEvents();

  void update(float elapsedTime);
  void draw();

  std::string m_titile;
  int m_screenWidth;
  int m_screenHeight;

  Playable *m_game;
};

#endif
