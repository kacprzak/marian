#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "../actors/Actor.h"
#include "../events/EventType.h"
#include "ActorComponent.h"

#include <Box2D/Box2D.h>

enum ActorPhysicsStateId { IDLE = 1, RUN, FALL };

//==============================================================================

class PhysicsComponent : public ActorComponent
{
    friend class ActorFactory;

  public:
    PhysicsComponent()
        : ActorComponent()
        , m_body(nullptr)
    {
    }

    ~PhysicsComponent()
    {
        if (m_body) {
            m_body->GetUserData().pointer = 0; // To silence contact listener
            m_body->GetWorld()->DestroyBody(m_body);
        }
    }

    bool init()
    {
        if (!m_owner || !m_body)
            return false;

        m_body->GetUserData().pointer = m_owner->id();
        return true;
    }

    // TODO: Make private
    b2Body* body() { return m_body; }
    void setBody(b2Body* body)
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
        m_body->ApplyLinearImpulse(b2Vec2(x, y), m_body->GetWorldCenter(),
                                   true);
    }

    /*!
     * \brief Handle contact with some other object.
     *
     * \param other      object that contacts this object
     * \param fixtureUD  user data of fixture that collided with other
     */
    virtual void handleBeginContact(Actor& /*other*/,
                                    uintptr_t /*fixtureUD*/ = 0)
    {
    }
    virtual void handleEndContact(Actor& /*other*/, uintptr_t /*fixtureUD*/ = 0)
    {
    }

    virtual void handleInputCommand(event::InputCommand /*command*/) {}

    ActorComponentId componentId() const { return PHYSICS; }

  protected:
    b2Body* m_body;
};

#endif // PHYSICSCOMPONENT_H
