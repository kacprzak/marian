/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "ActorComponent.h"
#include "actors/Actor.h"
#include "events/EventType.h"

#include <Box2D/Box2D.h>

enum ActorPhysicsStateId {
    STAND = 1,
    RUN,
    FALL
};

//==============================================================================

class PhysicsComponent : public ActorComponent
{
    friend class ActorFactory;

 public:
    PhysicsComponent()
        : ActorComponent()
        , m_body(nullptr)
    {}

    ~PhysicsComponent()
    {
        if (m_body) {
            m_body->SetUserData(nullptr); // To silence contact listener
            m_body->GetWorld()->DestroyBody(m_body);
        }
    }


    bool init()
    {
        if (!m_owner || !m_body)
            return false;

        m_body->SetUserData(reinterpret_cast<void *>(m_owner->id()));
        return true;
    }


    // TODO: Make private
    b2Body *body() { return m_body; }
    void setBody(b2Body *body)
    {
        assert(m_body == nullptr);
        m_body = body;
    }

    float posX() const { return m_body->GetPosition().x; }
    float posY() const { return m_body->GetPosition().y; }

    float angle() const { return m_body->GetAngle(); }

    float velX() const { return m_body->GetLinearVelocity().x; }
    float velY() const { return m_body->GetLinearVelocity().y; }

    void applyForceToCenter(float x, float y)
    {
        m_body->ApplyForceToCenter(b2Vec2(x, y), true);
    }

    void applyLinearImpulse(float x, float y)
    {
        m_body->ApplyLinearImpulse(b2Vec2(x, y), m_body->GetWorldCenter(), true);
    }

    /*!
     * \brief Handle contact with some other object.
     *
     * \param other      object that contacts this object
     * \param fixtureUD  user data of fixture that collided with other
     */
    virtual void handleBeginContact(ActorPtr /*other*/,
                                    void * /*fixtureUD*/ = nullptr) {}
    virtual void handleEndContact  (ActorPtr /*other*/,
                                    void * /*fixtureUD*/ = nullptr) {}

    virtual void handleInputCommand(event::InputCommand /*command*/) {}

    ActorComponentId componentId() const { return PHYSICS; }

 protected:
    b2Body *m_body;
};

#endif // PHYSICSCOMPONENT_H
