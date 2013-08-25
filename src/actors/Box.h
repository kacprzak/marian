/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef BOX_H
#define BOX_H

#include "components/PhysicsComponent.h"
#include "components/ImageRenderComponent.h"

class GameLogic;

class BoxPhysicsComponent : public PhysicsComponent
{
 public:
    BoxPhysicsComponent(GameLogic *game, float x, float y,
                        float w = 1.0f, float h = 1.0f);
};

class BoxRenderComponent : public ImageRenderComponent
{
 public:
    BoxRenderComponent();
};

#endif
