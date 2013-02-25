/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Box.h"

#include "Engine.h"
#include "Game.h"
#include <Box2D/Box2D.h>

Box::Box(Game *game, const MapObject& obj, const Image& image)
    : GameObject(game, nullptr)
    , m_image(image)
{
    float hw = obj.width / 2;
    float hh = obj.height / 2;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    // Set origin in center
    bodyDef.position.Set(obj.x + hw, obj.y + hh);
    //bodyDef.fixedRotation = true;
    b2Body* body = game->world()->CreateBody(&bodyDef);
    
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(hw, hh);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.8f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.4f;
    fixtureDef.userData = this;

    body->CreateFixture(&fixtureDef);

    m_body = body;
}

//------------------------------------------------------------------------------

void Box::draw(Engine *e)
{
    const b2Vec2& pos = m_body->GetPosition();
    float w = 1.0f;
    float h = 1.0f;
    e->drawImage(pos.x - w/2, pos.y - h/2,
                 w, h, m_body->GetAngle(), m_image);
}
