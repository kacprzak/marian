/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef RENDERER_H
#define RENDERER_H

#include "graphics/Image.h"

#include <GL/gl.h>
#include <GL/glu.h>

class Renderer
{
 public:
    virtual ~Renderer() {}

    virtual void drawImage(const Image& image, float x, float y, float rotation = 0.0f) const = 0;
    virtual void drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                          GLuint textureId, const GLfloat *texCoords) const = 0;

};

#endif // RENDERER_H
