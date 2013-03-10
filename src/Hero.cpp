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
    , m_boxesInContact(0)
{
    // Image creation
    const Texture *tex = ResourceMgr::instance().getTexture("MegaMan_001.png");
    if (!tex) {
        ResourceMgr::instance().addTexture("MegaMan_001.png");
        tex = ResourceMgr::instance().getTexture("MegaMan_001.png");
    }

    int ax = 38;
    int ax_off = 36;
    m_animation.setReversable(true);
    m_animation.addFrame(Image(tex, ax, 255, ax + 32, 255 + 32), 1.0f);
    m_animation.addFrame(Image(tex, ax + ax_off, 255, ax + ax_off + 32, 255 + 32), 0.1f); // Blink

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
    e->centerViewOn(centerOfMass.x, centerOfMass.y);

    if (m_jumpTimeout > 0.0f)
        m_jumpTimeout -= elapsedTime;
}

//------------------------------------------------------------------------------

void Hero::draw(Engine *e)
{
    const b2Vec2& pos = m_body->GetPosition();
    e->drawImage(m_animation.currentFrame(), pos.x, pos.y + 0.5f, 2);
}

//------------------------------------------------------------------------------

void Hero::handleBeginContact(GameObject *other)
{
    if (other->category() != SENSOR) return;

    if (m_boxesInContact == 0)
        std::cout << "is touching" << std::endl;
    
    ++m_boxesInContact;
}

//------------------------------------------------------------------------------

void Hero::handleEndContact(GameObject *other)
{
    if (other->category() != SENSOR) return;

    --m_boxesInContact;

    if (m_boxesInContact == 0)
        std::cout << "is not touching" << std::endl;
}
