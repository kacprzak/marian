#ifndef GROUND_H
#define GROUND_H

#include "../Map.h"
#include "../components/PhysicsComponent.h"

class GameLogic;

class GroundPhysicsComponent : public PhysicsComponent
{
  public:
    GroundPhysicsComponent(GameLogic* game, const MapObject& obj);
};

#endif
