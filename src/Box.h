/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef BOX_H
#define BOX_H

#include "GameObject.h"

#include "Map.h"
#include "Image.h"

class Box : public GameObject
{
 public:
    Box(Game *game, const MapObject& obj, const Image& image);

    void draw(Engine *e);

 private:
    Image m_image;
};

#endif
