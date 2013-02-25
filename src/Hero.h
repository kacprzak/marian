/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef HERO_H
#define HERO_H

#include "GameObject.h"

#include "Map.h"
#include "Image.h"

class Engine;

class Hero : public GameObject
{
 public:
    Hero(Game *game, const MapObject& obj, const Image& image);
  
    void update(Engine *e, float elapsedTime);
    void draw(Engine *e);

 private:
    float m_jumpTimeout;
    Image m_image;
};

#endif
