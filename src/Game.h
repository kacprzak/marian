/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAME_H
#define GAME_H

#include "Playable.h"
#include <vector>
#include "Map.h"
#include "FpsCounter.h"

#include <Box2D/Box2D.h>
#include "debugdraw/Render.h"

class GameObject;

class Game : public Playable
{
 public:
    Game();
    ~Game();

    // Playable interface
    void initialize(Engine *e);

    bool processInput(const SDL_Event& event);
    void update(Engine *e, float elapsedTime);
    void draw(Engine *e);

    Map *map() { return &m_map; }
    b2World *world() { return m_world; }

 private:
    void toggleDrawDebug();

    std::vector<GameObject *> m_gameObjects;
    Map m_map;

    b2World   *m_world;
    DebugDraw *m_debugDraw;
    bool       m_drawDebugData;

    FpsCounter m_fpsCounter;
};

#endif
