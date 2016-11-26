/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*-
 */
#ifndef ACTORFACTORY_H
#define ACTORFACTORY_H

#include "Actor.h"
#include "Map.h"

class GameLogic;

class ActorFactory
{
  public:
    static ActorPtr create(GameLogic *game, const MapObject &obj);

    static ActorPtr create(GameLogic *game, ActorCategory type,
                           const std::string &name, float x, float y);

  private:
    static unsigned long getNextId();
};

#endif // ACTORFACTORY_H
