/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>

template<typename T>
struct Rect
{
    Rect() {}

Rect(T aLeft, T aBottom, T aRight, T aTop)
: left(aLeft), bottom(aBottom), right(aRight), top(aTop)
        {}

    T left, bottom, right, top;
};

std::vector<int> hexColorToRgb(int hexValue);

std::vector<int> hexColorToRgb(const std::string& hexString);

std::string appendDirSeparator(const std::string& folder);

#endif
