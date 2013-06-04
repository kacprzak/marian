/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "ActorComponent.h"
#include <Box2D/Box2D.h>


class PhysicsComponent : public ActorComponent
{
    friend class PhysicsFactory;

 public:
    PhysicsComponent()
        : ActorComponent()
        , m_body(nullptr)
    {}

    ~PhysicsComponent() {
        if (m_body) {
            m_body->SetUserData(nullptr); // To silence contact listener
            m_body->GetWorld()->DestroyBody(m_body);
        }
    }

    b2Body *body() { return m_body; }
    void setBody(b2Body *body)
    {
        assert(m_body == nullptr);
        m_body = body;
    }

    float posX() const {
        return m_body->GetPosition().x;
    }

    float posY() const {
        return m_body->GetPosition().y;
    }

    float angle() const {
        return m_body->GetAngle();
    }

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


    ActorComponentId componentId() const { return PHYSICS; }

 protected:
    b2Body *m_body;
};

#endif // PHYSICSCOMPONENT_H
