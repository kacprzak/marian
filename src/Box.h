/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef BOX_H
#define BOX_H

#include "PhysicsComponent.h"
#include "ImageRenderComponent.h"

class Game;

class BoxPhysicsComponent : public PhysicsComponent
{
 public:
    BoxPhysicsComponent(Game *game, float x, float y,
                        float w = 1.0f, float h = 1.0f);
};

class BoxRenderComponent : public ImageRenderComponent
{
 public:
    BoxRenderComponent();
};

#endif
