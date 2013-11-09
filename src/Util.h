/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>

template<typename T>
struct Rect
{
    T left, right, top, bottom;
};

std::vector<int> hexColorToRgb(int hexValue);

std::vector<int> hexColorToRgb(std::string hexString);

#endif
