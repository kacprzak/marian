/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Hero.h"

#include "Engine.h"
#include "ResourceMgr.h"
#include "Game.h"
#include "graphics/Animation.h"
#include <iostream>
#include "Box2dPhysicsEngine.h"
#include "EventMgr.h"

#define JUMP_DELAY 0.5f
#define FEET_SENSOR 1248


HeroPhysicsComponent::HeroPhysicsComponent(GameLogic *game, float x, float y,
                                           float w, float h)
{
    // Physics
    float hw = w / 2;
    float hh = h / 2;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    // Set origin in center
    bodyDef.position.Set(x + hw, y + hh);
    bodyDef.fixedRotation = true;
    bodyDef.userData = this;

    Box2dPhysicsEngine *pe = static_cast<Box2dPhysicsEngine *>(game->physicsEngine());
    b2Body* body = pe->world()->CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = hw;

    b2FixtureDef fixtureDef;
    fixtureDef.filter.categoryBits = HERO;
    fixtureDef.filter.maskBits = GROUND | BOX | SENSOR;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 0.8f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.0f;

    // Main fixture
    body->CreateFixture(&fixtureDef);

    // Add foot sensor fixture
    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(hw, 0.3f, b2Vec2(0, -hh), 0);
    fixtureDef.shape = &polygonShape;
    fixtureDef.isSensor = true;
    fixtureDef.filter.maskBits = GROUND | BOX;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.0f;
    b2Fixture* footSensorFixture = body->CreateFixture(&fixtureDef);
    footSensorFixture->SetUserData(reinterpret_cast<void *>(FEET_SENSOR));

    m_body = body;

    // Other stuff
    m_feetContacts = 0;
    m_heroStateId = FALL;
    m_jumpTimeout = JUMP_DELAY;

    m_lastX = posX();
    m_lastY = posY();

    m_movingRight = false;
    m_movingLeft = false;
    m_jumping = false;
}

//------------------------------------------------------------------------------

void HeroPhysicsComponent::handleBeginContact(Actor *other, void *fixtureUD)
{
    if (fixtureUD == (void*)FEET_SENSOR) {
        ++m_feetContacts;
    }

    if (other->category() != SENSOR) return;

    std::cout << "is touching " << other->name() << std::endl;
}

//------------------------------------------------------------------------------

void HeroPhysicsComponent::handleEndContact(Actor *other, void *fixtureUD)
{
    if (fixtureUD == (void*)FEET_SENSOR) {
        --m_feetContacts;
    }

    if (other->category() != SENSOR) return;

    std::cout << "is not touching " << other->name() << std::endl;
}

//------------------------------------------------------------------------------

void HeroPhysicsComponent::update(float elapsedTime)
{
    if (m_movingRight) {
        applyForceToCenter(10.0f, 0.0f);
    }

    if (m_movingLeft) {
        applyForceToCenter(-10.0f, 0.0f);
    }

    if (m_jumping) {
        applyLinearImpulse(0.0f, 5.0f);
        m_jumping = false;
        m_jumpTimeout = JUMP_DELAY;
    }


    // Physical state management

    switch(m_heroStateId) {
    case FALL:
    {
        if (isOnGround() && std::abs(velY()) < 0.1f) {
            changeState(STAND);
        }
        break;
    }

    case RUN:
    {
        if (std::abs(velY()) > 0.1f) {
            changeState(FALL);
            break;
        }

        if (std::abs(velX()) < 0.5f) {
            changeState(STAND);
        }
        break;
    }

    case STAND:
    {
        if (std::abs(velY()) > 6.0f) {
            changeState(FALL);
            break;
        }

        if (std::abs(velX()) > 0.6f) {
            changeState(RUN);
        }
        break;
    }
    }

    // emit move event if position changed
    if (m_lastX != posX() || m_lastY != posY()) {
        EventMgr::singleton().queueEvent(EventPtr(new MoveEvent(m_owner->id(), posX(), posY())));
    }

    if (m_jumpTimeout > 0.0f)
        m_jumpTimeout -= elapsedTime;

    m_lastX = posX();
    m_lastY = posY();
}

//------------------------------------------------------------------------------

void HeroPhysicsComponent::changeState(ActorPhysicsStateId state)
{
    m_heroStateId = state;
    // emit event
    EventMgr::singleton().queueEvent(EventPtr(new PhysicsStateChangeEvent(m_owner->id(), state)));
}

//------------------------------------------------------------------------------

void HeroPhysicsComponent::handleInputCommand(InputCommand command)
{
    switch(command) {
    case MOVE_RIGHT_START: m_movingRight = true;  break;
    case MOVE_RIGHT_END:   m_movingRight = false; break;
    case MOVE_LEFT_START:  m_movingLeft  = true;  break;
    case MOVE_LEFT_END:    m_movingLeft  = false; break;
    case MOVE_DOWN_START:  break;
    case MOVE_DOWN_END:    break;
    case JUMP:             if (isOnGround() && m_jumpTimeout <= 0.0f) m_jumping = true; break;
    case FIRE:             break;
    }
}

