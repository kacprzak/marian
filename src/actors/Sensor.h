/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef SENSOR_H
#define SENSOR_H

#include "Ground.h"

class SensorPhysicsComponent : public GroundPhysicsComponent
{
public:
    SensorPhysicsComponent(GameLogic *game, const MapObject& obj);
};

#endif // SENSOR_H
