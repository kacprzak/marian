/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Playable.h"
#include "GameObjectCategory.h"
#include <Box2D/Box2D.h>
#include <string>
#include <iostream>

class Game;

class GameObject : public Playable
{
 public:
    GameObject(Game *game, b2Body *body = nullptr)
        : m_game(game)
        , m_body(body)
        , m_dead(false)
    {}

    virtual ~GameObject()
    {
#ifndef NDEBUG
        std::clog << "Removed: " << this << " name = " << name() << '\n';
#endif 
        if (m_body) {
            m_body->SetUserData(nullptr); // To silence contact listener
            m_body->GetWorld()->DestroyBody(m_body);
        }
    }

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

    void die() { m_dead = true; }
    bool dead() const { return m_dead; }

    void setName(const std::string& name) { m_name = name; }
    const std::string& name() const { return m_name; }

 protected:
    Game       *m_game;
    b2Body     *m_body;
    bool        m_dead;
    std::string m_name;
};

#endif // GAME_OBJECT_H

