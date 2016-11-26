#include "Texture.h"

#ifdef _MSC_VER
#include <windows.h>
#endif

#include <GL/gl.h>

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <stdexcept>

using namespace gfx;

static GLuint load_texture(SDL_Surface* surface, int* w, int* h);
static int SDL_InvertSurface(SDL_Surface* image);

//------------------------------------------------------------------------------

Texture::Texture()
    : m_textureId(0)
    , m_w(0)
    , m_h(0)
{
}

//------------------------------------------------------------------------------

Texture::~Texture() { glDeleteTextures(1, &m_textureId); }

//------------------------------------------------------------------------------

void Texture::loadFromFile(const std::string& filename)
{
    SDL_Surface* surface = IMG_Load(filename.c_str());

    if (!surface) {
        throw std::runtime_error("SDL_Image load error: " +
                                 std::string(IMG_GetError()));
    }

    loadFromSDL(surface);
    SDL_FreeSurface(surface);
}

//------------------------------------------------------------------------------

void Texture::loadFromSDL(SDL_Surface* surface)
{
    // Top down inversion
    if (SDL_InvertSurface(surface) != 0)
        throw std::runtime_error("SDL Error: " + std::string(SDL_GetError()));

    m_textureId = load_texture(surface, &m_w, &m_h);

    if (m_textureId == 0)
        throw std::runtime_error("Unable to load texture");
}

//==============================================================================

static GLuint load_texture(SDL_Surface* surface, int* w, int* h)
{
    GLuint textureid;
    int mode;
    int converted = false;

    // work out what format to tell glTexImage2D to use...
    if (surface->format->BytesPerPixel == 3) { // RGB 24bit
        mode = GL_RGB;
    } else if (surface->format->BytesPerPixel == 4) { // RGBA 32bit
        mode = GL_RGBA;
    } else {
        // Convert to 32 bits.
        SDL_PixelFormat fmt;
        memset(&fmt, 0, sizeof(fmt));
        fmt.format        = SDL_PIXELFORMAT_RGBA8888;
        fmt.BitsPerPixel  = 32;
        fmt.BytesPerPixel = 4;
        fmt.Rmask         = 0xff;
        fmt.Gmask         = 0xff00;
        fmt.Bmask         = 0xff0000;
        fmt.Amask         = 0xff000000;

        SDL_Surface* nimg = SDL_ConvertSurface(surface, &fmt, 0);

        if (!nimg) {
            std::cerr << "SDL error: " << SDL_GetError() << "\n";
            return 0;
        }

        // Done converting.
        converted = true;
        // surface is now pointing to object that needs to be freed!
        surface = nimg;
        mode    = GL_RGBA;
    }

    *w = surface->w;
    *h = surface->h;

    // Create one texture name
    glGenTextures(1, &textureid);

    // Tell opengl to use the generated texture name
    glBindTexture(GL_TEXTURE_2D, textureid);

    // This reads from the sdl surface and puts it into an opengl texture
    SDL_LockSurface(surface);
    glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode,
                 GL_UNSIGNED_BYTE, surface->pixels);
    SDL_UnlockSurface(surface);

    // These affect how this texture is drawn later on...
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Clamping
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Clean up
    if (converted)
        SDL_FreeSurface(surface);

    return textureid;
}

//------------------------------------------------------------------------------
// Code from
// http://www.gribblegames.com/articles/game_programming/sdlgl/invert_sdl_surfaces.html
//
static int invert_image(int pitch, int height, void* image_pixels)
{
    int index;
    void* temp_row;
    int height_div_2;

    temp_row = (void*)malloc(pitch);
    if (NULL == temp_row) {
        SDL_SetError("Not enough memory for image inversion");
        return -1;
    }
    // if height is odd, don't need to swap middle row
    height_div_2 = (int)(height * .5);
    for (index = 0; index < height_div_2; ++index) {
        // uses string.h
        memcpy((Uint8*)temp_row, (Uint8*)(image_pixels) + pitch * index, pitch);

        memcpy((Uint8*)(image_pixels) + pitch * index,
               (Uint8*)(image_pixels) + pitch * (height - index - 1), pitch);

        memcpy((Uint8*)(image_pixels) + pitch * (height - index - 1), temp_row,
               pitch);
    }
    free(temp_row);
    return 0;
}

//------------------------------------------------------------------------------

// This is the function you want to call!
static int SDL_InvertSurface(SDL_Surface* image)
{
    if (NULL == image) {
        SDL_SetError("Surface is NULL");
        return -1;
    }
    if (SDL_LockSurface(image) != 0) {
        return -1;
    }
    int err = invert_image(image->pitch, image->h, image->pixels);
    SDL_UnlockSurface(image);
    return err;
}
