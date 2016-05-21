/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GROUND_H
#define GROUND_H

#include "components/PhysicsComponent.h"
#include "Map.h"

class GameLogic;

class GroundPhysicsComponent : public PhysicsComponent
{
public:
    GroundPhysicsComponent(GameLogic *game, const MapObject& obj);
};

#endif
