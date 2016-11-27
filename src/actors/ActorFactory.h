#ifndef ACTORFACTORY_H
#define ACTORFACTORY_H

#include "Actor.h"
#include "Map.h"

class GameLogic;

class ActorFactory
{
  public:
    static std::unique_ptr<Actor> create(GameLogic* game, const MapObject& obj);

    static std::unique_ptr<Actor> create(GameLogic* game, ActorCategory type,
                                         const std::string& name, float x,
                                         float y);

  private:
    static unsigned long getNextId();
};

#endif // ACTORFACTORY_H
