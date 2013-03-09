/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Playable.h"

class Game;
class b2Body; // Box2D

class GameObject : public Playable
{
 public:
    GameObject(Game *game, b2Body *body = nullptr)
        : m_game(game)
        , m_body(body)
    {}

    virtual void handleBeginContact(GameObject * /*other*/) {}
    virtual void handleEndContact(GameObject * /*other*/) {}

    // Playable interface impl
    bool processInput(const SDL_Event& /*event*/) override { return true; }
    void update(Engine * /*e*/, float /*elapsedTime*/) override {}
    void draw(Engine * /*e*/) override {}

 protected:
    Game   *m_game;
    b2Body *m_body;
};

#endif
