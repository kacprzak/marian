/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef BOX_H
#define BOX_H

#include "GameObject.h"

#include "Map.h"
#include "Image.h"

class Box : public GameObject
{
 public:
    Box(Game *game, const Image& image,
        float x, float y,
        float w = 1.0f, float h = 1.0f);

    void draw(Engine *e) override;

 private:
    Image m_image;
};

#endif
