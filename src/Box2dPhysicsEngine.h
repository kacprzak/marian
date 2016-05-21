/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef BOX2DPHYSICSENGINE_H
#define BOX2DPHYSICSENGINE_H

#include "PhysicsEngine.h"
#include "debugdraw/Render.h"

#include <Box2D/Box2D.h>

#include <memory>

class ContactListener : public b2ContactListener
{
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;
};

//------------------------------------------------------------------------------

class Box2dPhysicsEngine : public PhysicsEngine
{
    public:
    Box2dPhysicsEngine();
    ~Box2dPhysicsEngine() override;

    void update(float elapsedTime) override;

    void toggleDrawDebug() override;
    void drawDebugData() override;

    //void applyForce(const Vec2& dir, float newtons, ActorId actorId) override;

    b2World *world() { return m_world.get(); }

    private:
    std::unique_ptr<b2World> m_world;
    ContactListener          m_contactListener;

    std::unique_ptr<DebugDraw> m_debugDraw;
    bool                       m_drawDebugData;

};

#endif // BOX2DPHYSICSENGINE_H
