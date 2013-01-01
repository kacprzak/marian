#include "Engine.h"

#include <iostream>
#include <cstdlib> // exit
#include <SDL.h>

#include "Texture.h"
#include <vector>
#include "ResourceMgr.h"

Engine::Engine(const std::string& title, int screenWidth, int screenHeight)
  : m_titile(title)
  , m_screenWidth(screenWidth)
  , m_screenHeight(screenHeight)
  , m_translate_x(0.0f)
  , m_translate_y(0.0f)
  , m_translate_z(0.0f)
  , m_scale(1.0f)
  , m_game(0)
{
  try {
    initializeSDL();
    initializeOpenGL();
    SDL_WM_SetCaption(title.c_str(), title.c_str());
  } catch (SdlError e) {
    SDL_Quit();
    throw e;
  }

  for (int i = 0; i < SDLK_LAST; ++i) {
    m_keys[i] = false;
  }
}

//------------------------------------------------------------------------------

Engine::~Engine()
{
  // Release all resources
  ResourceMgr::instance().release();

  std::cout << "Quitting SDL...\n";
  SDL_Quit();
}

//------------------------------------------------------------------------------

void Engine::mainLoop(Playable *game)
{
  m_game = game;

  unsigned int curr_time = SDL_GetTicks();
  unsigned int last_time = curr_time;
  float delta = 0.0f;

  for (;;) {
    if (!processEvents())
      break;
    update(delta);
    draw();
    
    curr_time = SDL_GetTicks();
    delta = (float)(curr_time - last_time) / 1000.0f;
    last_time = curr_time;
  }
}

//------------------------------------------------------------------------------

void Engine::centerOnPixel(float x, float y)
{
  m_translate_x = -x * m_scale;
  m_translate_y = -y * m_scale;
}

//------------------------------------------------------------------------------

/** Return true if should keep going. */
bool Engine::processEvents()
{
  SDL_Event event;
  
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_KEYUP:
      m_keys[event.key.keysym.sym] = false;
      return m_game->processInput(event);
    case SDL_KEYDOWN:
      m_keys[event.key.keysym.sym] = true;
      return m_game->processInput(event);
    case SDL_QUIT:
      return false;
    }
  }

  return true;
}

//------------------------------------------------------------------------------

void Engine::update(float elapsedTime)
{
  // Update world
  m_game->update(this, elapsedTime);
}

//------------------------------------------------------------------------------

void Engine::draw()
{
  //m_translate_z = -10.0f;
  //m_scale = 2.0f;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(m_translate_x,
               m_translate_y,
               m_translate_z);
  glScalef(m_scale, m_scale, 1.0f);

  m_game->draw(this);

  SDL_GL_SwapBuffers(); 
}

//------------------------------------------------------------------------------

void Engine::drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h)
{
  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(x + w, y);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(x + w, y + h);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + h);
  glEnd();
}

//------------------------------------------------------------------------------

void Engine::drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat texCoords[])
{
  glBegin(GL_QUADS);
    glTexCoord2f(texCoords[0], texCoords[1]); glVertex2f(x, y);
    glTexCoord2f(texCoords[2], texCoords[3]); glVertex2f(x + w, y);
    glTexCoord2f(texCoords[4], texCoords[5]); glVertex2f(x + w, y + h);
    glTexCoord2f(texCoords[6], texCoords[7]); glVertex2f(x, y + h);
  glEnd();
}

//------------------------------------------------------------------------------

void Engine::drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLuint texture_id, GLfloat texCoords[])
{
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  // Enable texturing if needed.
  bool texturing_enabled = glIsEnabled(GL_TEXTURE_2D);
  if(!texturing_enabled)
    glEnable(GL_TEXTURE_2D);

  // Bind texture and draw.
  glBindTexture(GL_TEXTURE_2D, texture_id);
  
  drawQuad(x, y, w, h, texCoords);

  // Disable if was disable.
  if(!texturing_enabled)
    glDisable(GL_TEXTURE_2D);

  glDisable(GL_BLEND);
}

//------------------------------------------------------------------------------

void Engine::drawSprite(const Sprite& sprite)
{
  const Texture *tex = sprite.texture();
  GLfloat texCoords[8];
  sprite.getTextureCoords(texCoords);

  drawQuad(sprite.position().x,
	   sprite.position().y,
	   sprite.width(),
	   sprite.height(),
	   tex->textureId(),
           texCoords);
}

//------------------------------------------------------------------------------

void Engine::initializeSDL()
{
  std::cout << "Initializing SDL...\n";

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw SdlError("Could not initialize SDL", SDL_GetError());  
  }

  /* Some video inforamtion */
  const SDL_VideoInfo *info = SDL_GetVideoInfo();
  
  if (!info) {
    throw SdlError("Video query failed", SDL_GetError());
  }

  int screen_bpp = info->vfmt->BitsPerPixel;

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);
  //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

  char env[] = "SDL_VIDEO_CENTERED=center";
  SDL_putenv(env);

  int screen_flags = SDL_OPENGL;

  // Drawing surface
  SDL_Surface *screen = SDL_SetVideoMode(m_screenWidth, m_screenHeight,
					 screen_bpp, screen_flags);

  if (!screen) {
    throw SdlError("Setting video mode failed", SDL_GetError());
  }

  SDL_ShowCursor(SDL_DISABLE);
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);  

  std::cout << "SDL initialized.\n";
}

//------------------------------------------------------------------------------

void Engine::initializeOpenGL()
{
  std::cout << "Initializing OpenGl...\n";
  //float ratio = float(m_screenWidth) / float(m_screenHeight);

  int r = 102, g = 140, b = 255;

  //glShadeModel(GL_SMOOTH); // smooth is default
  glClearColor(r/255.0f, g/255.0f, b/255.0f, 0);
  glViewport(0, 0, m_screenWidth, m_screenHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluPerspective(60.0, ratio, 1.0, 1024.0);
  //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glOrtho(-m_screenWidth/2, m_screenWidth/2, -m_screenHeight/2, m_screenHeight/2, -1, 1);

  glDisable(GL_DEPTH_TEST); // uncomment this if going 2D
}
