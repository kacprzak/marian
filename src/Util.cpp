/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Util.h"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

// Code from
// http://www.gribblegames.com/articles/game_programming/sdlgl/invert_sdl_surfaces.html

int invert_image(int pitch, int height, void* image_pixels)
{
    int index;
    void* temp_row;
    int height_div_2;

    temp_row = (void *)malloc(pitch);
    if(NULL == temp_row) {
        SDL_SetError("Not enough memory for image inversion");
        return -1;
    }
    //if height is odd, don't need to swap middle row
    height_div_2 = (int) (height * .5);
    for(index = 0; index < height_div_2; index++) {
        //uses string.h
        memcpy((Uint8 *)temp_row,
               (Uint8 *)(image_pixels) +
               pitch * index,
               pitch);

        memcpy((Uint8 *)(image_pixels) +
               pitch * index,
               (Uint8 *)(image_pixels) +
               pitch * (height - index-1),
               pitch);

        memcpy((Uint8 *)(image_pixels) +
               pitch * (height - index-1),
               temp_row,
               pitch);
    }
    free(temp_row);
    return 0;
}

//This is the function you want to call!
int SDL_InvertSurface(SDL_Surface* image)
{
    if(NULL == image) {
        SDL_SetError("Surface is NULL");
        return -1;
    }
    return( invert_image(image->pitch, image->h,
                         image->pixels) );
}

// My code

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

    // Top down inversion
    SDL_InvertSurface(surface);

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Clamping
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Clean up
    SDL_FreeSurface(surface);

    std::cout << "Loaded texture from: " << filename << std::endl;

    return textureid;
}

void calculateTextureCoords(float texWidth, float texHeight,
                            const Rect<int>& rect, GLfloat *texCoords)
{
    if (!rect.isNull()) {
        GLfloat s0 = rect.left()   / texWidth;
        GLfloat t0 = rect.bottom() / texHeight;
        GLfloat s1 = rect.right()  / texWidth;
        GLfloat t1 = rect.top()    / texHeight;

        texCoords[0] = s0; texCoords[1] = t0;
        texCoords[2] = s1; texCoords[3] = t0;
        texCoords[4] = s1; texCoords[5] = t1;
        texCoords[6] = s0; texCoords[7] = t1;      
    } else {
        texCoords[0] = 0.0f; texCoords[1] = 0.0f;
        texCoords[2] = 1.0f; texCoords[3] = 0.0f;
        texCoords[4] = 1.0f; texCoords[5] = 1.0f;
        texCoords[6] = 0.0f; texCoords[7] = 1.0f;
    }
}

std::vector<int> hexColorToRgb(int hexValue)
{
    std::vector<int> rgbColor(3, 0); // Black

    rgbColor[0] = ((hexValue >> 16) & 0xFF);  // Extract the RR byte
    rgbColor[1] = ((hexValue >> 8) & 0xFF);   // Extract the GG byte
    rgbColor[2] = ((hexValue) & 0xFF);        // Extract the BB byte

    return rgbColor; 
}

std::vector<int> hexColorToRgb(std::string hexString)
{
    if (hexString.front() == '#')
        hexString.erase(0, 1);

    int hexValue = std::stoi(hexString, nullptr, 16);;

    return hexColorToRgb(hexValue); 
}
