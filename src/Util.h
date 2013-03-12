/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef UTIL_H
#define UTIL_H

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <GL/gl.h>
#include <vector>
#include <string>

GLuint load_texture(const char *filename, int *w, int *h);

void calculateTextureCoords(GLfloat texCoords[8],
                            int texWidth, int texHeight,
                            int x0 = 0, int y0 = 0, int x1 = 0, int y1 = 0);

void flipVerticallyTextureCoords(GLfloat texCoords[8]);

void flipHorizontallyTextureCoords(GLfloat texCoords[8]);

std::vector<int> hexColorToRgb(int hexValue);

std::vector<int> hexColorToRgb(std::string hexString);

#endif
