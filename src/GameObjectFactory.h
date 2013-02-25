/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H

#include "GameObject.h"
#include "Map.h"
#include <cassert>

#include "Hero.h"
#include "Ground.h"
#include "Box.h"

class GameObjectFactory
{
 public:
    static GameObject *create(Game *game, const MapObject& obj)
    {
        GameObject *go = nullptr;

        if (obj.name == "hero") {
            // Hero
            const Image& img = game->map()->imageForTile(348);
            go = new Hero(game, obj, img);

        } else if (obj.type == "Box") {
            const Image& img = game->map()->imageForTile(237);
            go = new Box(game, obj, img);
            
        } else {
            // Static collision shape
            go = new Ground(game, obj);  
        }
        assert(go);
        return go;
    }
};

#endif
