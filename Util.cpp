#include "Util.h"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

GLuint load_texture(const char *filename, int *w, int *h)
{
  SDL_Surface *surface;
  GLuint textureid;
  int mode;

  surface = IMG_Load(filename);

  if (!surface) {
    std::cerr << "Could not load " << filename << std::endl;
    return 0;
  }

  // work out what format to tell glTexImage2D to use...
  if (surface->format->BytesPerPixel == 3) { // RGB 24bit
    mode = GL_RGB;
  } else if (surface->format->BytesPerPixel == 4) { // RGBA 32bit
    mode = GL_RGBA;
  } else {
    // Convert to 32 bits.
    SDL_PixelFormat fmt = {
      NULL, 32, 4, 
      0, 0, 0, 0,
      0, 8, 16, 24,
      0xff, 0xff00, 0xff0000, 0xff000000,
      0,
      0xff
    };

    SDL_Surface *nimg = SDL_ConvertSurface(surface, &fmt, SDL_SWSURFACE);
    SDL_FreeSurface(surface);

    if(!nimg) {
      std::cerr << "Could not convert image "
		<< filename << " to 32-bit\n";
      return 0;
    }

    // Done converting.
    surface = nimg;
    mode = GL_RGBA;
  }

  *w = surface->w;
  *h = surface->h;

  // Create one texture name
  glGenTextures(1, &textureid);

  // Tell opengl to use the generated texture name
  glBindTexture(GL_TEXTURE_2D, textureid);

  // This reads from the sdl surface and puts it into an opengl texture
  glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0,
	       mode, GL_UNSIGNED_BYTE, surface->pixels);

  // These affect how this texture is drawn later on...
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  // Clean up
  SDL_FreeSurface(surface);

  std::cout << "Loaded texture from: " << filename << std::endl;

  return textureid;
}
