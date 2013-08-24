/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef SENSOR_H
#define SENSOR_H

#include "Ground.h"

class SensorPhysicsComponent : public GroundPhysicsComponent
{
 public:
    SensorPhysicsComponent(Game *game, const MapObject& obj);
};

#endif // SENSOR_H
