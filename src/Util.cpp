/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Util.h"

std::vector<int> hexColorToRgb(int hexValue)
{
    std::vector<int> rgbColor(3, 0); // Black

    rgbColor[0] = ((hexValue >> 16) & 0xFF);  // Extract the RR byte
    rgbColor[1] = ((hexValue >> 8) & 0xFF);   // Extract the GG byte
    rgbColor[2] = ((hexValue) & 0xFF);        // Extract the BB byte

    return rgbColor; 
}

//------------------------------------------------------------------------------

std::vector<int> hexColorToRgb(const std::string& str)
{
    std::string hexString = str;

    if (hexString.front() == '#')
        hexString.erase(0, 1);

    int hexValue = std::stoi(hexString, nullptr, 16);;

    return hexColorToRgb(hexValue); 
}

//------------------------------------------------------------------------------

std::string appendDirSeparator(const std::string &folder)
{
    const char dirSep = '/';

    std::string retFolder = folder;
    if (retFolder.back() != dirSep)
        retFolder.push_back(dirSep);

    return retFolder;
}
