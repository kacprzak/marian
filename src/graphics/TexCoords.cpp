/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "TexCoords.h"

namespace gfx {

TexCoords<4> calculateTexCoords(int texWidth, int texHeight, const Rect<int>& tileCoords)
{
    TexCoords<4> tc;

    // Left bottom
    tc[0].s = tileCoords.left   / static_cast<float>(texWidth);
    tc[0].t = tileCoords.bottom / static_cast<float>(texHeight);

    // Top right
    tc[2].s = tileCoords.right / static_cast<float>(texWidth);
    tc[2].t = tileCoords.top   / static_cast<float>(texHeight);

    // Right bottom
    tc[1].s = tc[2].s;
    tc[1].t = tc[0].t;

    // Left top
    tc[3].s = tc[0].s;
    tc[3].t = tc[2].t;

    return tc;
}

//------------------------------------------------------------------------------

TexCoords<4> flipVerticallyTexCoords(const TexCoords<4>& texCoords)
{
    TexCoords<4> tc;

    tc[0] = texCoords[1];
    tc[1] = texCoords[0];
    tc[2] = texCoords[3];
    tc[3] = texCoords[2];

    return tc;
}

//------------------------------------------------------------------------------

TexCoords<4> flipHorizontallyTexCoords(const TexCoords<4>& texCoords)
{
    TexCoords<4> tc;

    tc[0] = texCoords[3];
    tc[1] = texCoords[2];
    tc[2] = texCoords[1];
    tc[3] = texCoords[0];

    return tc;
}

} // namespace gfx
