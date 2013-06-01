/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "ActorComponent.h"

class PhysicsComponent : public ActorComponent
{
    friend class PhysicsFactory;

 public:
    virtual ~PhysicsComponent();

    ActorComponentId componentId() const { return PHYSICS; }
};

#endif // PHYSICSCOMPONENT_H
