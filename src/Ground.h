/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GROUND_H
#define GROUND_H

#include "Actor.h"

#include "Map.h"

class Ground : public Actor
{
 public:
    Ground(unsigned long id, Game *game, const MapObject& obj);

    ActorCategory category() override
    { return GROUND; }
};

#endif
