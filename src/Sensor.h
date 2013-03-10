/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef SENSOR_H
#define SENSOR_H

#include "GameObject.h"

#include "Map.h"
#include "Ground.h"

class Sensor : public Ground
{
 public:
    Sensor(Game *game, const MapObject& obj);

    GameObjectCategory category() override
    { return SENSOR; }
};

#endif // SENSOR_H
