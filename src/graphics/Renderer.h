/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef RENDERER_H
#define RENDERER_H

#include "graphics/Image.h"

namespace gfx {

/*!
 * \brief The Renderer interface.
 */
class Renderer
{
 public:
    virtual ~Renderer() {}

    virtual void drawImage(const Image& image, float x, float y,
                           float rotation = 0.0f) const = 0;
    virtual void drawQuad(float x, float y, float w, float h,
                          unsigned textureId, const float *texCoords) const = 0;

};

} // namespace gfx

#endif // RENDERER_H
