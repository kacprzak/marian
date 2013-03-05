/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef UTIL_H
#define UTIL_H

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <GL/gl.h>
#include <vector>
#include <string>
#include "Vect.h"

GLuint load_texture(const char *filename, int *w, int *h);

void calculateTextureCoords(int texWidth, int texHeight,
                            const Rect<int>& rect, GLfloat *texCoords);

std::vector<int> hexColorToRgb(int hexValue);

std::vector<int> hexColorToRgb(std::string hexString);

#endif
