/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef HERO_H
#define HERO_H

#include "GameObject.h"

#include "Map.h"
#include "StateMachine.h"
#include <memory>

class Engine;

class Hero : public GameObject
{
 public:
    Hero(Game *game, float x, float y, float w, float h);
    ~Hero();
  
    void update(Engine *e, float elapsedTime) override;
    void draw(Engine *e) override;

    GameObjectCategory category() override
    { return HERO; }

    void handleBeginContact(GameObject *other) override;
    void handleEndContact(GameObject *other) override;

 private:
    float m_jumpTimeout;
    StateMachine<Hero *> m_stateMachine;
    std::vector<State<Hero *> *> m_states;
};

#endif
