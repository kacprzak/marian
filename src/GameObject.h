/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Playable.h"

#include <Box2D/Box2D.h>

class Game;

class GameObject : public Playable
{
 public:
    explicit GameObject(Game *game, b2Body *body)
        : m_game(game)
        , m_body(body)
    {}

    const b2Vec2& position() const {
        return m_body->GetPosition();
    }

    // Playable interface impl
    bool processInput(const SDL_Event& event);
    void update(Engine *e, float elapsedTime);
    void draw(Engine *e);

 protected:
    Game *m_game;
    b2Body* m_body;
};

#endif
