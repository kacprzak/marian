/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef SENSOR_H
#define SENSOR_H

#include "Actor.h"

#include "Map.h"
#include "Ground.h"

class Sensor : public Ground
{
 public:
    Sensor(unsigned long id, Game *game, const MapObject& obj);

    ActorCategory category() override
    { return SENSOR; }
};

#endif // SENSOR_H
