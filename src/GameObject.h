/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Playable.h"

class Game;
class b2Body; // Box2D

class GameObject : public Playable
{
 public:
    GameObject(Game *game, b2Body *body)
        : m_game(game)
        , m_body(body)
    {}

    // Playable interface impl
    bool processInput(const SDL_Event& /*event*/) { return true; };
    void update(Engine * /*e*/, float /*elapsedTime*/) {};
    void draw(Engine * /*e*/) {};

 protected:
    Game   *m_game;
    b2Body *m_body;
};

#endif
