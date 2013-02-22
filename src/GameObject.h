/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Playable.h"
#include "Sprite.h"

class Game;

class GameObject : public Playable
{
 public:
    explicit GameObject(Game *game, const Sprite& sprite)
        : m_game(game)
        , m_sprite(sprite)
    {}

    Vector2<float> position() const { return m_sprite.position(); }

    // Playable interface impl
    bool processInput(const SDL_Event& event);
    void update(Engine *e, float elapsedTime);
    void draw(Engine *e);

 protected:
    Game *m_game;
    Sprite m_sprite;
    Rect<float> collisionRect;

};

#endif
