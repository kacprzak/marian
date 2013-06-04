/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GROUND_H
#define GROUND_H

#include "PhysicsComponent.h"
#include "Map.h"

class Game;

class GroundPhysicsComponent : public PhysicsComponent
{
 public:
    GroundPhysicsComponent(Game *game, const MapObject& obj);
};

#endif
