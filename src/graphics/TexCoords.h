/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef TEXCOORDS_H
#define TEXCOORDS_H

#include <cstddef> // size_t

/* Texture coords utils. */

/** Single texture coord. S and T have bounds [0, 1] */
struct TexCoord
{
    float s, t;
};

template <std::size_t size>
struct TexCoords
{
    TexCoord coords[size];
};


TexCoords<4> calculateTexCoords(int texWidth, int texHeight,
                                int x0 = 0, int y0 = 0, int x1 = 0, int y1 = 0);
TexCoords<4> flipVerticallyTexCoords(const TexCoords<4>& texCoords);
TexCoords<4> flipHorizontallyTexCoords(const TexCoords<4>& texCoords);

#endif // TEXCOORDS_H
