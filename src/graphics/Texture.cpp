/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Texture.h"

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <GL/gl.h>

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

static GLuint load_texture(const char *filename, int *w, int *h);
static int SDL_InvertSurface(SDL_Surface *image);

//------------------------------------------------------------------------------

Texture::Texture()
    : m_textureId(0)
    , m_w(0)
    , m_h(0)
{}

//------------------------------------------------------------------------------

Texture::~Texture()
{
    glDeleteTextures(1, &m_textureId);
}

//------------------------------------------------------------------------------

bool Texture::loadFromFile(const std::string& filename)
{
    m_textureId = load_texture(filename.c_str(), &m_w, &m_h);
    return m_textureId == 0 ? false : true;
}

//==============================================================================

static GLuint load_texture(const char *filename, int *w, int *h)
{
    SDL_Surface *surface;
    GLuint textureid;
    int mode;

    surface = IMG_Load(filename);

    if (!surface) {
        std::cerr << "Could not load " << filename << std::endl;
        return 0;
    }

    // Top down inversion
    SDL_InvertSurface(surface);

    // work out what format to tell glTexImage2D to use...
    if (surface->format->BytesPerPixel == 3) { // RGB 24bit
        mode = GL_RGB;
    } else if (surface->format->BytesPerPixel == 4) { // RGBA 32bit
        mode = GL_RGBA;
    } else {
        // Convert to 32 bits.
        SDL_PixelFormat fmt;
        memset(&fmt, 0, sizeof(fmt));
        fmt.format = SDL_PIXELFORMAT_RGBA8888;
        fmt.BitsPerPixel = 32;
        fmt.BytesPerPixel = 4;
        fmt.Rmask = 0xff;
        fmt.Gmask = 0xff00;
        fmt.Bmask = 0xff0000;
        fmt.Amask = 0xff000000;

        SDL_Surface *nimg = SDL_ConvertSurface(surface, &fmt, 0);
        SDL_FreeSurface(surface);

        if(!nimg) {
            std::cerr << "Could not convert image " << filename << " to 32-bit\n";
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Clamping
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Clean up
    SDL_FreeSurface(surface);

    return textureid;
}

//------------------------------------------------------------------------------
// Code from
// http://www.gribblegames.com/articles/game_programming/sdlgl/invert_sdl_surfaces.html

static int invert_image(int pitch, int height, void* image_pixels)
{
    int index;
    void *temp_row;
    int height_div_2;

    temp_row = (void *)malloc(pitch);
    if(NULL == temp_row) {
        SDL_SetError("Not enough memory for image inversion");
        return -1;
    }
    //if height is odd, don't need to swap middle row
    height_div_2 = (int)(height * .5);
    for (index = 0; index < height_div_2; ++index) {
        //uses string.h
        memcpy((Uint8 *)temp_row,
               (Uint8 *)(image_pixels) + pitch * index,
               pitch);

        memcpy((Uint8 *)(image_pixels) + pitch * index,
               (Uint8 *)(image_pixels) + pitch * (height - index-1),
               pitch);

        memcpy((Uint8 *)(image_pixels) + pitch * (height - index-1),
               temp_row,
               pitch);
    }
    free(temp_row);
    return 0;
}

//------------------------------------------------------------------------------

//This is the function you want to call!
static int SDL_InvertSurface(SDL_Surface *image)
{
    if(NULL == image) {
        SDL_SetError("Surface is NULL");
        return -1;
    }
    return invert_image(image->pitch, image->h, image->pixels);
}
