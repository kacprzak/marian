#ifndef ENGINE_H
#define ENGINE_H

#include <GL/gl.h>
#include <GL/glu.h>

#include <string>
#include <exception>
#include "Playable.h"
#include "Sprite.h"

#include <Box2D/Box2D.h>

class SdlError : public std::exception
{
 public:
  SdlError(const std::string& msg, char *sdlError)
    : m_msg(msg)
    , m_sdlError(sdlError)
  {
    //
  }

  ~SdlError() throw() {}

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

  void drawSprite(const Sprite& sprite) const;
  void drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                GLuint textureId, const GLfloat *texCoords) const;

  void centerOnPixel(float x, float y);

  bool isPressed(int keycode) const { return m_keys[keycode]; }
  int screenWidth() const  { return m_screenWidth; }
  int screenHeight() const { return m_screenHeight; }

 private:
  void drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h) const;
  void drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h, const GLfloat *texCoords) const;
  void initializeSDL();
  void initializeOpenGL();
  void initializeWorld();

  bool processEvents();

  void update(float elapsedTime);
  void draw();

  std::string m_titile;
  int m_screenWidth;
  int m_screenHeight;

  float m_translate_x;
  float m_translate_y;
  float m_translate_z;
  float m_scale;

  Playable *m_game;

  bool m_keys[SDLK_LAST];

  b2World *m_world;
};

#endif
