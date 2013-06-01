/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef ACTOR_H
#define ACTOR_H

#include "Playable.h"
#include "ActorCategory.h"
#include <Box2D/Box2D.h>
#include <string>
#include <iostream>
#include "ActorFactory.h"
#include "ActorComponent.h"

class Game;

class Actor final : public Playable
{
    friend class ActorFactory;

 public:
    explicit Actor(ActorId id, Game *game)
        : m_id(id)
        , m_game(game)
        , m_body(nullptr)
        , m_dead(false)
    {}

    virtual ~Actor()
    {
#ifndef NDEBUG
        std::clog << "Actor removed: id = " << m_id << " name = " << name() << '\n';
#endif 
        if (m_body) {
            m_body->SetUserData(nullptr); // To silence contact listener
            m_body->GetWorld()->DestroyBody(m_body);
        }
    }

    virtual ActorCategory category() = 0;

    /**
     * Handle contact with some other object.
     *
     * @param other      object that contacts this object
     * @param fixtureUD  user data of fixture that collided with other
     */
    virtual void handleBeginContact(Actor * /*other*/,
                                    void * /*fixtureUD*/ = nullptr) {}
    virtual void handleEndContact  (Actor * /*other*/,
                                    void * /*fixtureUD*/ = nullptr) {}

    // Playable interface impl
    bool processInput(const SDL_Event& /*event*/) override { return true; }
    void update(Engine * /*e*/, float /*elapsedTime*/) override {}
    void draw(Engine * /*e*/) override {}

    b2Body *body() { return m_body; }
    void setBody(b2Body *body)
    {
        assert(m_body == nullptr);
        m_body = body;
    }

    void die() { m_dead = true; }
    bool dead() const { return m_dead; }

    void setName(const std::string& name) { m_name = name; }
    const std::string& name() const { return m_name; }

 protected:
    ActorId     m_id;
    Game       *m_game;
    b2Body     *m_body;
    bool        m_dead;
    std::string m_name;
};

#endif // ACTOR_H

