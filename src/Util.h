#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

template <typename T>
struct Rect
{
    Rect() {}

    Rect(T aLeft, T aBottom, T aRight, T aTop)
        : left(aLeft)
        , bottom(aBottom)
        , right(aRight)
        , top(aTop)
    {
    }

    T left, bottom, right, top;
};

std::vector<int> hexColorToRgb(int hexValue);

std::vector<int> hexColorToRgb(const std::string& hexString);

std::string appendDirSeparator(const std::string& folder);

#endif
