/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef UTIL_H
#define UTIL_H

#include <GL/gl.h>
#include <vector>
#include <string>
#include "Math.h"

GLuint load_texture(const char *filename, int *w, int *h);

void calculateTextureCoords(float texWidth, float texHeight,
                            const Rect<int>& rect, GLfloat *texCoords);

std::vector<int> hexColorToRgb(int hexValue);

std::vector<int> hexColorToRgb(std::string hexString);

#endif
