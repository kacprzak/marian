#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "Renderer.h"

namespace gfx {

class GLRenderer : public Renderer
{
  public:
    ~GLRenderer() override {}

    // Renderer interface
    void drawImage(const Image& image, float x, float y,
                   float rotation) const override;
    void drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLuint textureId,
                  const GLfloat* texCoords) const override;

  private:
    void drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h) const;
    void drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                  const GLfloat* texCoords) const;
};

} // namespace gfx

#endif // GLRENDERER_H
