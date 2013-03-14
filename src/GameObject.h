/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Playable.h"
#include "GameObjectCategory.h"

class Game;
class b2Body; // Box2D

class GameObject : public Playable
{
 public:
    GameObject(Game *game, b2Body *body = nullptr)
        : m_game(game)
        , m_body(body)
    {}

    virtual GameObjectCategory category() = 0;

    /**
     * Handle contact with some other object.
     *
     * @param other      object that contacts this object
     * @param fixtureUD  user data of fixture that collided with other
     */
    virtual void handleBeginContact(GameObject * /*other*/,
                                    void * /*fixtureUD*/ = nullptr) {}
    virtual void handleEndContact  (GameObject * /*other*/,
                                    void * /*fixtureUD*/ = nullptr) {}

    // Playable interface impl
    bool processInput(const SDL_Event& /*event*/) override { return true; }
    void update(Engine * /*e*/, float /*elapsedTime*/) override {}
    void draw(Engine * /*e*/) override {}

    b2Body *body() { return m_body; }

 protected:
    Game   *m_game;
    b2Body *m_body;
};

#endif // GAME_OBJECT_H

