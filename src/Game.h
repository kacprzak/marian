/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAME_H
#define GAME_H

#include "Playable.h"
#include "Map.h"
#include "FpsCounter.h"
#include "Actor.h"
#include "ActorCategory.h"
#include <list>
#include <string>

#include <Box2D/Box2D.h>
#include "debugdraw/Render.h"

//class Actor;

class ContactListener : public b2ContactListener
{
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;
};

//------------------------------------------------------------------------------

class Game : public Playable
{
 public:
    Game();
    ~Game();

    // Playable interface
    void initialize(Engine *e) override;

    bool processInput(const SDL_Event& event) override;
    void update(Engine *e, float elapsedTime) override;
    void draw(Engine *e) override;

    Map *map() { return &m_map; }
    b2World *world() { return m_world; }

    void addGameObject(ActorCategory type,
                       const std::string& name,
                       float x, float y);

    bool isOnMap(ActorPtr actor);

 private:
    void toggleDrawDebug();

    std::map<ActorId, ActorPtr> m_actors;
    Map m_map;

    b2World   *m_world;
    DebugDraw *m_debugDraw;
    bool       m_drawDebugData;
    ContactListener m_contactListener;

    FpsCounter m_fpsCounter;
};

#endif
