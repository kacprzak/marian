#ifndef SENSOR_H
#define SENSOR_H

#include "Ground.h"

class SensorPhysicsComponent : public GroundPhysicsComponent
{
  public:
    SensorPhysicsComponent(GameLogic* game, const MapObject& obj);
};

#endif // SENSOR_H
