#include "Box2dPhysicsEngine.h"

#include "config.h"

#if PLATFORM == PLATFORM_WINDOWS
#include <windows.h>
#endif

#include "Logger.h"
#include "events/EventMgr.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <cassert>

using namespace event;

Box2dPhysicsEngine::Box2dPhysicsEngine()
    : m_world()
{
    // Debug drawing
    m_debugDraw.reset(new DebugDraw);
    m_drawDebugData = false;

    b2Vec2 gravity(0.0f, -9.8f);
    m_world.reset(new b2World(gravity));
    m_world->SetAllowSleeping(true);
    m_world->SetContactListener(&m_contactListener);

    LOG << "created Box2dPhysicsEngine (Box2d ver: " << b2_version.major << "."
        << b2_version.minor << "." << b2_version.revision << ")\n";
}

//------------------------------------------------------------------------------

Box2dPhysicsEngine::~Box2dPhysicsEngine()
{
    // assert(!m_world);

    LOG << "destroyed Box2dPhysicsEngine\n";
}

//------------------------------------------------------------------------------

void Box2dPhysicsEngine::update(float elapsedTime)
{
    // Update Physics
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    m_world->Step(elapsedTime, velocityIterations, positionIterations);
}

//------------------------------------------------------------------------------

void Box2dPhysicsEngine::toggleDrawDebug()
{
    if (m_drawDebugData == false) {
        m_debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);
        m_world->SetDebugDraw(m_debugDraw.get());
    } else {
        m_world->SetDebugDraw(nullptr);
    }

    m_drawDebugData = !m_drawDebugData;
}

//------------------------------------------------------------------------------

void Box2dPhysicsEngine::drawDebugData()
{
    if (m_drawDebugData) {
        glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);

        glDisable(GL_TEXTURE_2D);
        m_world->DebugDraw();

        glPopAttrib();
    }
}

//------------------------------------------------------------------------------

// void Box2dPhysicsEngine::applyForce(const Vec2& dir, float newtons, ActorId
// actorId)
//{}

//------------------------------------------------------------------------------

void ContactListener::BeginContact(b2Contact* contact)
{
    auto fixAUserData = contact->GetFixtureA()->GetUserData().pointer;
    auto fixBUserData = contact->GetFixtureB()->GetUserData().pointer;

    auto bodyAUserData =
        contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    auto bodyBUserData =
        contact->GetFixtureB()->GetBody()->GetUserData().pointer;

    if (bodyAUserData && bodyBUserData) {
        ActorId actorA = bodyAUserData;
        ActorId actorB = bodyBUserData;

        EventMgr& evtMgr = EventMgr::singleton();
        evtMgr.queueEvent(std::make_unique<CollisionEvent>(
            CollisionEvent::BEGIN, actorA, fixAUserData, actorB, fixBUserData));
    }
}

//------------------------------------------------------------------------------

void ContactListener::EndContact(b2Contact* contact)
{
    auto fixAUserData = contact->GetFixtureA()->GetUserData().pointer;
    auto fixBUserData = contact->GetFixtureB()->GetUserData().pointer;

    auto bodyAUserData =
        contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    auto bodyBUserData =
        contact->GetFixtureB()->GetBody()->GetUserData().pointer;

    if (bodyAUserData && bodyBUserData) {
        ActorId actorA = bodyAUserData;
        ActorId actorB = bodyBUserData;

        EventMgr& evtMgr = EventMgr::singleton();
        evtMgr.queueEvent(std::make_unique<CollisionEvent>(
            CollisionEvent::END, actorA, fixAUserData, actorB, fixBUserData));
    }
}
