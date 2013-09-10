/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Box2dPhysicsEngine.h"

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include "events/EventMgr.h"
#include "Logger.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <cassert>

Box2dPhysicsEngine::Box2dPhysicsEngine()
    : m_world(nullptr)
{
    LOG << "Box2dPhysicsEngine created\n";

    // Debug drawing
    m_debugDraw = new DebugDraw;
    m_drawDebugData = false;
}

//------------------------------------------------------------------------------

Box2dPhysicsEngine::~Box2dPhysicsEngine()
{
    assert(m_world == nullptr);

    delete m_debugDraw;

    LOG << "Box2dPhysicsEngine destroyed\n";
}

//------------------------------------------------------------------------------

bool Box2dPhysicsEngine::init()
{
    b2Vec2 gravity(0.0f, -9.8f);
    m_world = new b2World(gravity);
    m_world->SetAllowSleeping(true);
    m_world->SetContactListener(&m_contactListener);

    return true;
}

//------------------------------------------------------------------------------

void Box2dPhysicsEngine::shutdown()
{
    delete m_world;
    m_world = nullptr;
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
        m_world->SetDebugDraw(m_debugDraw);    
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
        m_world->DrawDebugData();

        glPopAttrib();
    }
}

//------------------------------------------------------------------------------

//void Box2dPhysicsEngine::applyForce(const Vec2& dir, float newtons, ActorId actorId)
//{}

//------------------------------------------------------------------------------

void ContactListener::BeginContact(b2Contact *contact)
{
    void *fixAUserData = contact->GetFixtureA()->GetUserData();
    void *fixBUserData = contact->GetFixtureB()->GetUserData();

    void *bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
    void *bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();

    if ( bodyAUserData && bodyBUserData ) {
        ActorId actorA = reinterpret_cast<ActorId>(bodyAUserData);
        ActorId actorB = reinterpret_cast<ActorId>(bodyBUserData);

        EventPtr e(new CollisionEvent(CollisionEvent::BEGIN,
                                      actorA, fixAUserData,
                                      actorB, fixBUserData));
        EventMgr::singleton().queueEvent(e);
    }
}

//------------------------------------------------------------------------------

void ContactListener::EndContact(b2Contact *contact)
{
    void *fixAUserData = contact->GetFixtureA()->GetUserData();
    void *fixBUserData = contact->GetFixtureB()->GetUserData();

    void *bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
    void *bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();

    if ( bodyAUserData && bodyBUserData ) {
        ActorId actorA = reinterpret_cast<ActorId>(bodyAUserData);
        ActorId actorB = reinterpret_cast<ActorId>(bodyBUserData);

        EventPtr e(new CollisionEvent(CollisionEvent::END,
                                      actorA, fixAUserData,
                                      actorB, fixBUserData));
        EventMgr::singleton().queueEvent(e);
    }
}
