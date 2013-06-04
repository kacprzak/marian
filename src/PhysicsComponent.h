/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "ActorComponent.h"

class PhysicsComponent : public ActorComponent
{
    friend class PhysicsFactory;

 public:
    virtual ~PhysicsComponent();

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
};

#endif // PHYSICSCOMPONENT_H
