/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Playable.h"
#include "Sprite.h"

#include <Box2D/Box2D.h>

class Game;

class GameObject : public Playable
{
 public:
    explicit GameObject(Game *game, const Sprite& sprite, b2Body *body)
        : m_game(game)
        , m_sprite(sprite)
        , m_body(body)
    {}

    Vector2<float> position() const {
        const b2Vec2& pos = m_body->GetPosition();
        return Vector2<float>(pos.x, pos.y);
    }

    // Playable interface impl
    bool processInput(const SDL_Event& event);
    void update(Engine *e, float elapsedTime);
    void draw(Engine *e);

 protected:
    Game *m_game;
    Sprite m_sprite;
    b2Body* m_body;

};

#endif
