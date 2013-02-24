/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef HERO_H
#define HERO_H

#include "GameObject.h"

#include "Image.h"
#include <Box2D/Box2D.h>

class Engine;

class Hero : public GameObject
{
 public:
    Hero(Game *game, const Image& image,
         const b2Vec2& pos, const b2Vec2& size);
  
    void update(Engine *e, float elapsedTime);
    void draw(Engine *e);

 private:
    float m_jumpTimeout;
    Image m_image;
};

#endif
