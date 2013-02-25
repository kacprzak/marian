/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GROUND_H
#define GROUND_H

#include "GameObject.h"

#include "Map.h"

class Ground : public GameObject
{
 public:
    Ground(Game *game, const MapObject& obj);
};

#endif
