/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef HERO_H
#define HERO_H

#include "GameObject.h"

class Engine;

class Hero : public GameObject
{
 public:
    Hero(Engine *e, Game *game, const Sprite& sprite, const b2Vec2& pos);
  
    void update(Engine *e, float elapsedTime);
};

#endif
