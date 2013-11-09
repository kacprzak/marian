/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef TEXCOORDS_H
#define TEXCOORDS_H

#include "Util.h"

#include <cstddef> // size_t
#include <iostream>

/* Texture coords utils. */

/** Single texture coord. S and T have bounds [0, 1] */
struct TexCoord
{
    float s, t;
};

inline bool operator==(const TexCoord& lhs, const TexCoord& rhs)
{
    return (lhs.s == rhs.s && lhs.t == rhs.t);
}

inline bool operator!=(const TexCoord& lhs, const TexCoord& rhs)
{
    return (lhs.s != rhs.s || lhs.t != rhs.t);
}

inline std::ostream& operator<<(std::ostream& os, const TexCoord& tc)
{
    return os << "{" << tc.s << " " << tc.t << "}";
}

//==============================================================================

template <std::size_t size>
struct TexCoords
{
    TexCoord coords[size];
};

template<std::size_t size>
bool operator==(const TexCoords<size>& lhs, const TexCoords<size>& rhs)
{
    for (std::size_t i = 0; i < size; ++i) {
        if (lhs.coords[i] != rhs.coords[i])
            return false;
    }

    return true;
}

template<std::size_t size>
std::ostream& operator<<(std::ostream& os, const TexCoords<size>& tc)
{
    os << "{";
    for (std::size_t i = 0; i < size; ++i) {
        const TexCoord& c = tc.coords[i];
        os << c;
    }
    os << "}";

    return os;
}

//------------------------------------------------------------------------------

TexCoords<4> calculateTexCoords(int texWidth, int texHeight, const Rect<int>& tileCoords);
TexCoords<4> flipVerticallyTexCoords(const TexCoords<4>& texCoords);
TexCoords<4> flipHorizontallyTexCoords(const TexCoords<4>& texCoords);

#endif // TEXCOORDS_H
