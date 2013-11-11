/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "TexCoords.h"


TexCoords<4> calculateTexCoords(int texWidth, int texHeight, const Rect<int>& tileCoords)
{
    TexCoords<4> cs;

    // Left bottom
    cs.coords[0].s = tileCoords.left   / static_cast<float>(texWidth);
    cs.coords[0].t = tileCoords.bottom / static_cast<float>(texHeight);

    // Top right
    cs.coords[2].s = tileCoords.right / static_cast<float>(texWidth);
    cs.coords[2].t = tileCoords.top   / static_cast<float>(texHeight);

    // Right bottom
    cs.coords[1].s = cs.coords[2].s;
    cs.coords[1].t = cs.coords[0].t;

    // Left top
    cs.coords[3].s = cs.coords[0].s;
    cs.coords[3].t = cs.coords[2].t;

    return cs;
}

//------------------------------------------------------------------------------

TexCoords<4> flipVerticallyTexCoords(const TexCoords<4>& texCoords)
{
    TexCoords<4> cs;

    cs.coords[0] = texCoords.coords[1];
    cs.coords[1] = texCoords.coords[0];
    cs.coords[2] = texCoords.coords[3];
    cs.coords[3] = texCoords.coords[2];

    return cs;
}

//------------------------------------------------------------------------------

TexCoords<4> flipHorizontallyTexCoords(const TexCoords<4>& texCoords)
{
    TexCoords<4> cs;

    cs.coords[0] = texCoords.coords[3];
    cs.coords[1] = texCoords.coords[2];
    cs.coords[2] = texCoords.coords[1];
    cs.coords[3] = texCoords.coords[0];

    return cs;
}
