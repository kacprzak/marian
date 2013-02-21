#ifndef UTIL_H
#define UTIL_H

#include <GL/gl.h>
#include "Math.h"

GLuint load_texture(const char *filename, int *w, int *h);

void calculateTextureCoords(float texWidth, float texHeight,
                            const Rect<int>& rect, GLfloat *texCoords);

#endif
