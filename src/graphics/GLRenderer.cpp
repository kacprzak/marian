/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*-
 */
#include "GLRenderer.h"

#include "graphics/Texture.h"

#include <boost/math/constants/constants.hpp>

using namespace gfx;

//------------------------------------------------------------------------------

void GLRenderer::drawImage(const Image &image, float x, float y,
                           float rotation) const
{
    float deg = rotation * 180.0f / boost::math::constants::pi<float>();
    float hw  = image.width() / 2.0f;
    float hh  = image.height() / 2.0f;

    const Texture *tex       = image.texture();
    const GLfloat *texCoords = image.getTextureCoords();

    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(deg, 0.0f, 0.0f, 1.0f);

    drawQuad(-hw, -hh, image.width(), image.height(), tex->textureId(),
             texCoords);

    glPopMatrix();
}

//------------------------------------------------------------------------------

void GLRenderer::drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                          GLuint texture_id, const GLfloat *texCoords) const
{
    // Enable texturing if needed.
    GLboolean texturing_enabled = glIsEnabled(GL_TEXTURE_2D);
    if (!texturing_enabled)
        glEnable(GL_TEXTURE_2D);

    // Bind texture and draw.
    glBindTexture(GL_TEXTURE_2D, texture_id);

    drawQuad(x, y, w, h, texCoords);

    // Disable if it was disabled.
    if (!texturing_enabled)
        glDisable(GL_TEXTURE_2D);
}

//------------------------------------------------------------------------------

void GLRenderer::drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h) const
{
    // Disable texturing if needed.
    GLboolean texturing_enabled = glIsEnabled(GL_TEXTURE_2D);
    if (texturing_enabled)
        glDisable(GL_TEXTURE_2D);

    GLfloat vertices[8] = {x, y, x + w, y, x + w, y + h, x, y + h};

    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vertices);

    glDrawArrays(GL_QUADS, 0, 4);

    if (texturing_enabled)
        glEnable(GL_TEXTURE_2D);
}

//------------------------------------------------------------------------------

void GLRenderer::drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                          const GLfloat *texCoords) const
{
    static GLfloat s_texCoords[8] = {0.0f, 0.0f, 1.0f, 0.0f,
                                     1.0f, 1.0f, 0.0f, 1.0f};

    if (texCoords == nullptr)
        texCoords = s_texCoords;

    GLfloat vertices[8] = {x, y, x + w, y, x + w, y + h, x, y + h};

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    glVertexPointer(2, GL_FLOAT, 0, vertices);

    glDrawArrays(GL_QUADS, 0, 4);
}
