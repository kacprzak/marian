/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "Actor.h"

class Vec2;

class PhysicsEngine
{
 public:
    virtual ~PhysicsEngine() {}

    virtual bool init() = 0;
    virtual void shutdown() = 0;

    virtual void update(float elapsedTime) = 0;

    virtual void toggleDrawDebug() = 0;
    virtual void drawDebugData() = 0;
    
    virtual void applyForce(const Vec2& dir, float newtons, ActorId actorId) = 0;
};

#endif // PHYSICSENGINE_H
