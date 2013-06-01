/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef ACTORFACTORY_H
#define ACTORFACTORY_H

//#include "Actor.h"
//#include "ActorComponent.h"
#include "Map.h"
#include "Hero.h"
#include "Ground.h"
#include "Box.h"
#include "Sensor.h"

#include <cassert>
#include <memory>

class Actor;
class ActorComponent;

typedef unsigned long ActorId;
typedef std::shared_ptr<Actor> ActorPtr;
typedef std::shared_ptr<ActorComponent> ActorComponentPtr;

class ActorFactory
{
 public:
    static Actor *create(Game *game, const MapObject& obj)
    {
        Actor *actor = nullptr;

        if (obj.type == "Hero") {
            actor = new Hero(getNextId(), game, obj.x, obj.y, obj.width, obj.height);

        } else if (obj.type == "Box") {
            actor = new Box(getNextId(), game, obj.x, obj.y, obj.width, obj.height);
            
        } else if (obj.type == "Sensor") {
            actor = new Sensor(getNextId(), game, obj);
  
        } else {
            // Static collision shape
            actor = new Ground(getNextId(), game, obj);
        }
        assert(actor);
        actor->setName(obj.name);
        return actor;
    }

    //--------------------------------------------------------------------------

    static Actor *create(Game *game,
                         ActorCategory type,
                         const std::string& name,
                         float x, float y)
    {
        Actor *actor = nullptr;

        if (type == BOX) {
            actor = new Box(getNextId(), game, x, y);
        }
        assert(actor);
        actor->setName(name);
        return actor;
    }

    static unsigned long getNextId()
    {
        static unsigned long id = 0;
        ++id;
        return id;
    }

};

#endif // ACTORFACTORY_H
