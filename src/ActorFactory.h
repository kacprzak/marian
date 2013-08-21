/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef ACTORFACTORY_H
#define ACTORFACTORY_H

#include "Actor.h"
#include "ActorComponent.h"
#include "Map.h"
#include "actors/Hero.h"
#include "actors/Ground.h"
#include "actors/Box.h"
//#include "Sensor.h"

#include <cassert>
#include <memory>

class Game;

class ActorFactory
{
 public:
    static ActorPtr create(Game *game, const MapObject& obj)
    {
        ActorPtr actor(new Actor(getNextId(), game));

        if (obj.type == "Hero") {
            actor->setCategory(HERO);

            ActorComponentPtr physics(
                        new HeroPhysicsComponent(game, obj.x, obj.y, obj.width, obj.height));
            actor->addComponent(physics);
            physics->setOwner(actor);
            physics->init();

            ActorComponentPtr render(new HeroRenderComponent);
            actor->addComponent(render);
            render->setOwner(actor);
            render->init();

        } else if (obj.type == "Box") {
            actor->setCategory(BOX);

            ActorComponentPtr physics(
                new BoxPhysicsComponent(game, obj.x, obj.y, obj.width, obj.height));
            actor->addComponent(physics);
            physics->setOwner(actor);
            physics->init();

            ActorComponentPtr render(new BoxRenderComponent);
            actor->addComponent(render);
            render->setOwner(actor);
            render->init();

        } else if (obj.type == "Sensor") {
            //actor = new Sensor(getNextId(), game, obj);
  
        } else {
            // Static collision shape
            ActorComponentPtr physics(
                new GroundPhysicsComponent(game, obj));
            actor->addComponent(physics);
            physics->setOwner(actor);
            physics->init();
        }
        //assert(actor);
        actor->setName(obj.name);
        return actor;
    }

    //--------------------------------------------------------------------------

    static ActorPtr create(Game *game,
                           ActorCategory type,
                           const std::string& name,
                           float x, float y)
    {
        ActorPtr actor(new Actor(getNextId(), game));

        if (type == BOX) {
            actor->setCategory(BOX);

            ActorComponentPtr physics(
                new BoxPhysicsComponent(game, x, y));
            actor->addComponent(physics);
            physics->setOwner(actor);
            physics->init();

            ActorComponentPtr render(new BoxRenderComponent());
            actor->addComponent(render);
            render->setOwner(actor);
            render->init();
        }
        //assert(actor);
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
