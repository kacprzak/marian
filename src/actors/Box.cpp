/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Box.h"

#include "Engine.h"
#include "GameLogic.h"
#include "ResourceMgr.h"
#include <Box2D/Box2D.h>
#include "Box2dPhysicsEngine.h"
#include "EventMgr.h"

BoxPhysicsComponent::BoxPhysicsComponent(GameLogic *game, float x, float y,
                                         float w, float h)
{
    // Physics
    float hw = w / 2;
    float hh = h / 2;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    // Set origin in center
    bodyDef.position.Set(x + hw, y + hh);
    //bodyDef.fixedRotation = true;
    //bodyDef.userData = this;

    Box2dPhysicsEngine *pe = static_cast<Box2dPhysicsEngine *>(game->physicsEngine());
    b2Body* body = pe->world()->CreateBody(&bodyDef);
    
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(hw, hh);
    
    b2FixtureDef fixtureDef;
    fixtureDef.filter.categoryBits = BOX;
    fixtureDef.filter.maskBits = GROUND | BOX | HERO;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.8f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.4f;

    body->CreateFixture(&fixtureDef);

    m_body = body;

    m_lastX = posX();
    m_lastY = posY();
    m_lastAngle = angle();
}

//------------------------------------------------------------------------------

void BoxPhysicsComponent::update(float)
{
    // emit move event if position changed
    if (m_lastX != posX() || m_lastY != posY() || m_lastAngle != angle()) {
        EventMgr::singleton().queueEvent(EventPtr(new MoveEvent(m_owner->id(), posX(), posY(), angle())));
    }

    m_lastX = posX();
    m_lastY = posY();
    m_lastAngle = angle();
}

//------------------------------------------------------------------------------

BoxRenderComponent::BoxRenderComponent()
    : ImageRenderComponent()
{
    // Image creation
    const Texture *tex =
        ResourceMgr::singleton().getTexture("minecraft_tiles_big.png");
    m_image = std::unique_ptr<Image>(new Image(tex, 256, 480, 288, 512));
}
