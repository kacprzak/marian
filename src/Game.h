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

#include "PhysicsEngine.h"
#include "EventManager.h"

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
    PhysicsEngine *physicsEngine() { return m_physicsEngine; }

    void addGameObject(ActorCategory type,
                       const std::string& name,
                       float x, float y);

    bool isOnMap(ActorPtr actor);

    void handleActorCollidedEvent(EventPtr event);
    void handleActorPhysicsStateChanged(EventPtr event);
    void handleActorMoved(EventPtr event);

 private:
    void toggleDrawDebug();

    std::map<ActorId, ActorPtr> m_actors;
    Map m_map;

    PhysicsEngine *m_physicsEngine;

    FpsCounter m_fpsCounter;
};

#endif
