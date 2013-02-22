/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef HERO_H
#define HERO_H

#include "GameObject.h"

class Engine;

class Hero : public GameObject
{
 public:
    explicit Hero(Game *game, const Sprite& sprite, b2Body *body)
        : GameObject(game, sprite, body)
    {
        //
    }
  
    void update(Engine *e, float elapsedTime);
};

#endif
