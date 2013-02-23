/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Hero.h"

#include "Engine.h"
#include <iostream>

Hero::Hero(Engine *e, Game *game, const Sprite& sprite, const b2Vec2& pos)
    : GameObject(game, sprite, nullptr)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = pos;
    bodyDef.fixedRotation = true;
    b2Body* body = e->world()->CreateBody(&bodyDef);
    
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, 0.5f), 0.0f);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.userData = this;

    body->CreateFixture(&fixtureDef);

    m_body = body;
}

void Hero::update(Engine *e, float /*elapsedTime*/)
{
    if (e->isPressed(SDLK_RIGHT)) {
        m_body->ApplyForceToCenter(b2Vec2(10.0f, 0.0f));
    }

    if (e->isPressed(SDLK_LEFT)) {
        m_body->ApplyForceToCenter(b2Vec2(-10.0f, 0.0f));
    }

    if (e->isPressed(SDLK_UP)) {
        m_body->ApplyForceToCenter(b2Vec2(0.0f, 20.0f));
    } 

    const b2Vec2& centerOfMass = m_body->GetWorldCenter(); 

    // Center view on player
    e->centerViewOn(centerOfMass.x, centerOfMass.y);
}

//------------------------------------------------------------------------------
