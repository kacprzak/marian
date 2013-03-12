/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Hero.h"

#include "Engine.h"
#include "ResourceMgr.h"
#include "Game.h"
#include <iostream>
#include <Box2D/Box2D.h>

#define JUMP_DELAY 1.0f

Hero::Hero(Game *game, float x, float y, float w, float h)
    : GameObject(game)
    , m_jumpTimeout(0.0f)
    , m_stateMachine(nullptr, 0)
{
    m_stateMachine.setOwner(this);

    // Image creation
    const Texture *tex = ResourceMgr::instance().getTexture("MegaMan_001.png");
    if (!tex) {
        ResourceMgr::instance().addTexture("MegaMan_001.png");
        tex = ResourceMgr::instance().getTexture("MegaMan_001.png");
    }

    int ax = 38;
    int ax_off = 36;
    m_animation.setReversable(true);
    Image idleFrame1(tex, ax, 255, ax + 32, 255 + 32);
    idleFrame1.scale(2.0f);
    m_animation.addFrame(idleFrame1, 1.0f);
    Image idleFrame2(tex, ax + ax_off, 255, ax + ax_off + 32, 255 + 32);
    idleFrame2.scale(2.0f);
    m_animation.addFrame(idleFrame2, 0.1f); // Blink

    // Physics
    float hw = w / 2;
    float hh = h / 2;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    // Set origin in center
    bodyDef.position.Set(x + hw, y + hh);
    bodyDef.fixedRotation = true;
    bodyDef.userData = this;
    b2Body* body = game->world()->CreateBody(&bodyDef);
    
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(hw, hh);
    
    b2FixtureDef fixtureDef;
    fixtureDef.filter.categoryBits = category();
    fixtureDef.filter.maskBits = GROUND | BOX | SENSOR;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.8f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);

    m_body = body;
}

//------------------------------------------------------------------------------

void Hero::update(Engine *e, float elapsedTime)
{
    m_animation.update(elapsedTime);

    const b2Vec2& centerOfMass = m_body->GetWorldCenter(); 

    if (e->isPressed(SDLK_RIGHT)) {
        m_body->ApplyForceToCenter(b2Vec2(10.0f, 0.0f));
    }

    if (e->isPressed(SDLK_LEFT)) {
        m_body->ApplyForceToCenter(b2Vec2(-10.0f, 0.0f));
    }

    if (e->isPressed(SDLK_UP)) {
        if (m_jumpTimeout <= 0.0f) {
            m_body->ApplyLinearImpulse(b2Vec2(0.0f, 5.0f), centerOfMass);
            m_jumpTimeout = JUMP_DELAY;
        }
    } 

    // Center view on player
    float x = centerOfMass.x;
    float y = centerOfMass.y;

    // Respect map borders
    float bLeft, bRight, bTop, bBottom;
    e->viewBounds(&bLeft, &bRight, &bBottom, &bTop);
    float hw = (bRight - bLeft) / 2.0f;
    float hh = (bTop - bBottom) / 2.0f;
    if (x < hw) x = hw;
    if (y < hh) y = hh;
    if (x > m_game->map()->width() - hw) x = m_game->map()->width() - hw;

    e->centerViewOn(x, y);

    if (m_jumpTimeout > 0.0f)
        m_jumpTimeout -= elapsedTime;
}

//------------------------------------------------------------------------------

void Hero::draw(Engine *e)
{
    const b2Vec2& pos = m_body->GetPosition();
    e->drawImage(m_animation.currentFrame(), pos.x, pos.y + 0.5f);
}

//------------------------------------------------------------------------------

void Hero::handleBeginContact(GameObject *other)
{
    if (other->category() != SENSOR) return;

    std::cout << "is touching" << std::endl;
}

//------------------------------------------------------------------------------

void Hero::handleEndContact(GameObject *other)
{
    if (other->category() != SENSOR) return;

    std::cout << "is not touching" << std::endl;
}
