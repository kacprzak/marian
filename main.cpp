#include <iostream>
#include <cstdlib> // exit
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Texture.h"
#include "Sprite.h"
#include "ResourceMgr.h"
#include <vector>

// Globals
const char *title = "Marian";
const int screen_width = 640;
const int screen_height = 640;
std::vector<Sprite*> sprites;

void drawQuad(float x, float y, float w, float h)
{
  w /= 2.0f; h /= 2.0f;

  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glVertex2f(x - w, y - h);
  glTexCoord2f(1.0f, 0.0f); glVertex2f(x + w, y - h);
  glTexCoord2f(1.0f, 1.0f); glVertex2f(x + w, y + h);
  glTexCoord2f(0.0f, 1.0f); glVertex2f(x - w, y + h);
  glEnd();
}

void drawQuad(float x, float y, float w, float h, GLuint texture_id)
{
  // Enable texturing if needed.
  bool texturing_enabled = glIsEnabled(GL_TEXTURE_2D);
  if(!texturing_enabled)
    glEnable(GL_TEXTURE_2D);

  // Bind texture and draw.
  glBindTexture(GL_TEXTURE_2D, texture_id);
  drawQuad(x, y, w, h);

  // Disable if was disable.
  if(!texturing_enabled)
    glDisable(GL_TEXTURE_2D);
}

void drawSprite(const Sprite& sprite)
{
  const Texture *tex = sprite.texture();

  drawQuad(sprite.position().x,
	   sprite.position().y,
	   tex->w(),
	   tex->h(),
	   tex->textureId());
}

void quit(int code)
{
  std::cout << "Quitting SDL...\n";
  SDL_Quit();
  std::cout << "Quitting...\n";
  exit(code);
}

void initializeSDL()
{
  std::cout << "Initializing SDL...\n";

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Could not initialize SDL: " << SDL_GetError() << std::endl;  
    quit(-1);
  }

  /* Some video inforamtion */
  const SDL_VideoInfo *info = SDL_GetVideoInfo();
  
  if (!info) {
    std::cerr << "Video query faild: " << SDL_GetError() << std::endl;
    quit(-2);
  }

  int screen_bpp = info->vfmt->BitsPerPixel;

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  char env[] = "SDL_VIDEO_CENTERED=center";
  SDL_putenv(env);

  int screen_flags = SDL_OPENGL;

  // Drawing surface
  SDL_Surface *screen = SDL_SetVideoMode(screen_width, screen_height,
					 screen_bpp, screen_flags);

  if (!screen) {
    std::cerr << "Setting video mode faild: " << SDL_GetError() << std::endl;
    quit(-3);
  }

  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);  

  std::cout << "SDL initialized.\n";
}

void initializeOpenGL()
{
  float ratio = float(screen_width) / float(screen_height);

  glShadeModel(GL_SMOOTH);
  glClearColor(0, 0, 0, 0);
  glViewport(0, 0, screen_width, screen_height);
  glMatrixMode(GL_PROJECTION);
  gluPerspective(60.0, ratio, 1.0, 1024.0);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glDisable(GL_DEPTH_TEST); // uncomment this if going
}

void process_input(const SDL_Event& event)
{
  switch (event.type) {
  case SDL_KEYUP:
    std::cout << "Key Up: " << event.key.keysym.sym << std::endl;
    break;
  case SDL_KEYDOWN:
    std::cout << "Key Down: " << event.key.keysym.sym << std::endl;
    break;
  }
}

/** Return keep going? */
bool process_events() {
  SDL_Event event;
  
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_KEYUP:
    case SDL_KEYDOWN:
      process_input(event);
      break;
    case SDL_QUIT:
      return false;
    }
  }

  return true;
}

void update(float delta)
{
  // Update world
  Sprite *s = sprites[0];

  float v = 20.0f;
  
  float x = s->position().x;
  float y = s->position().y;

  float new_x = x + v * delta;

  s->setPosition(new_x, y);
}

void draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(0.0f, 0.0f, -10.0f);
  static float scale = 0.02f;
  glScalef(scale, scale, 1.0f);

  drawSprite(*sprites[0]);

  SDL_GL_SwapBuffers(); 
}

int main()
{
  initializeSDL();
  initializeOpenGL();

  if (!ResourceMgr::instance().addTexture("minecraft_tiles_big.png")) {
    std::cerr << "Unable to load texture.\n";
    quit(-4);
  }  

  const Texture *tex = ResourceMgr::instance().getTexture("minecraft_tiles_big.png");

  Sprite *sprite = new Sprite(tex);
  sprites.push_back(sprite);

  SDL_WM_SetCaption(title, title);

  unsigned int curr_time = SDL_GetTicks();
  unsigned int last_time = curr_time;
  float delta = 0.0f;

  for (;;) {
    if (!process_events())
      break;
    update(delta);
    draw();
    
    curr_time = SDL_GetTicks();
    delta = (float)(curr_time - last_time) / 1000.0f;
    last_time = curr_time;
  }

  delete sprite;
  quit(0);
}
