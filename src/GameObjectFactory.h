/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H

#include "GameObject.h"
#include "Map.h"
#include <cassert>

#include "Hero.h"
#include "Ground.h"
#include "Box.h"
#include "Sensor.h"

class GameObjectFactory
{
 public:
    static GameObject *create(Game *game, const MapObject& obj)
    {
        GameObject *go = nullptr;

        if (obj.type == "Hero") {
            go = new Hero(game, obj.x, obj.y, obj.width, obj.height);

        } else if (obj.type == "Box") {
            go = new Box(game, obj.x, obj.y, obj.width, obj.height);
            
        } else if (obj.type == "Sensor") {
            go = new Sensor(game, obj);
  
        } else {
            // Static collision shape
            go = new Ground(game, obj);  
        }
        assert(go);
        go->setName(obj.name);
        return go;
    }

    //--------------------------------------------------------------------------

    static GameObject *create(Game *game,
                              GameObjectCategory type,
                              const std::string& name,
                              float x, float y)
    {
        GameObject *go = nullptr;

        if (type == BOX) {
            go = new Box(game, x, y);
        }
        assert(go);
        go->setName(name);
        return go;
    }

};

#endif
