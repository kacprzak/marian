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
            go = new Hero(game, img,
                          obj.x, obj.y,
                          obj.width, obj.height);

        } else if (obj.type == "Box") {
            const Image& img = game->map()->imageForTile(237);
            go = new Box(game, img,
                         obj.x, obj.y,
                         obj.width, obj.height);            
        } else {
            // Static collision shape
            go = new Ground(game, obj);  
        }
        assert(go);
        return go;
    }

    //--------------------------------------------------------------------------

    static GameObject *create(Game *game,
                              const std::string& type,
                              const std::string& name,
                              float x, float y)
    {
        GameObject *go = nullptr;

        if (type == "Box") {
            const Image& img = game->map()->imageForTile(237);
            go = new Box(game, img, x, y);
        }
        assert(go);
        return go;
    }

};

#endif
