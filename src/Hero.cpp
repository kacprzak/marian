/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Hero.h"

#include "Engine.h"
#include "ResourceMgr.h"
#include "Game.h"
#include "Animation.h"
#include <iostream>
#include <Box2D/Box2D.h>

#define JUMP_DELAY 0.5f
#define FEET_SENSOR 1248

enum HeroStateId {
    STAND = 1,
    RUN,
    FALL
};

//==============================================================================

class StandHeroState : public GameObjectState
{
 public:
    StandHeroState(GameObjectStateMachine& stateMachine)
        : GameObjectState(stateMachine)
    {
        const Texture *tex = ResourceMgr::singleton().getTexture("MegaMan_001.png");

        int ax = 38;
        int ax_off = 36;
        int ay = 255;

        m_animation.setReversable(true);
        Image idleFrame1(tex, ax, ay, ax + 32, ay + 32);
        idleFrame1.scale(2.0f);
        m_animation.addFrame(idleFrame1, 1.0f);
        Image idleFrame2(tex, ax + ax_off, ay, ax + ax_off + 32, ay + 32);
        idleFrame2.scale(2.0f);
        m_animation.addFrame(idleFrame2, 0.1f); // Blink
    }

    void onEnter(GameObject * owner, int /*prevStateId*/) override
    {
        Hero *hero = static_cast<Hero *>(owner);
        setFacingRight(hero->isFacingRight());
    }

    void update(Engine * /*e*/, float elapsedTime) override
    {
        m_animation.update(elapsedTime);

        Hero *hero = static_cast<Hero *>(m_stateMachine.owner());
        const b2Vec2& vel = hero->body()->GetLinearVelocity();

        if (std::abs(vel.y) > 6.0f) {
            m_stateMachine.changeState(FALL);
            return;
        }

        if (std::abs(vel.x) > 0.6f) {
            m_stateMachine.changeState(RUN);
            return;
        }
    }

    void draw(Engine *e) override
    {
        const b2Vec2& pos = m_stateMachine.owner()->body()->GetPosition();
        e->drawImage(m_animation.currentFrame(), pos.x, pos.y + 0.5f);
    }

    void setFacingRight(bool right) 
    {
        static bool animFacingRight = true;
        if (animFacingRight != right) {
            m_animation.flipVertically();
            animFacingRight = right;
        }
    }

    Animation m_animation;
};

//==============================================================================

class FallHeroState : public GameObjectState
{
 public:
    FallHeroState(GameObjectStateMachine& stateMachine)
        : GameObjectState(stateMachine)
    {
        const Texture *tex = ResourceMgr::singleton().getTexture("MegaMan_001.png");

        int ax = 38;
        int ax_off = 4*36;
        int ay = 219;

        m_image = std::unique_ptr<Image>(new Image(tex, ax + ax_off, ay, ax + ax_off + 32, ay + 32));
        m_image->scale(2.0f);
    }

    void onEnter(GameObject * owner, int /*prevStateId*/) override
    {
        Hero *hero = static_cast<Hero *>(owner);
        setFacingRight(hero->isFacingRight());
    }

    void update(Engine * /*e*/, float /*elapsedTime*/) override
    {
        Hero *hero = static_cast<Hero *>(m_stateMachine.owner());
        const b2Vec2& vel = hero->body()->GetLinearVelocity();

        if (std::abs(vel.x) > 0.01f) {
            bool fr = (vel.x >= 0.0f);
            hero->setFacingRight(fr);
            setFacingRight(fr);
        }

        if (hero->isOnGround() && std::abs(vel.y) < 0.1f) {
            m_stateMachine.changeState(STAND);
            return;
        }
    }

    void draw(Engine *e) override
    {
        const b2Vec2& pos = m_stateMachine.owner()->body()->GetPosition();
        e->drawImage(*m_image, pos.x, pos.y + 0.5f);
    }

    void setFacingRight(bool right) 
    {
        static bool imageFacingRight = true;
        if (imageFacingRight != right) {
            m_image->flipVertically();
            imageFacingRight = right;
        }
    }

    std::unique_ptr<Image> m_image;
};

//==============================================================================

class RunHeroState : public GameObjectState
{
 public:
    RunHeroState(GameObjectStateMachine& stateMachine)
        : GameObjectState(stateMachine)
    {
        const Texture *tex = ResourceMgr::singleton().getTexture("MegaMan_001.png");

        int ax = 38 + 36;
        int ax_off = 36;
        int ay_off = 36;
        int ay = 219 - 36;
        float frameSpeed = 0.25f;
        m_animation.setReversable(true);

        int y = 1;
        for (int x = 2 ; x >= 0; --x) {
            Image runFrame(tex,
                           ax + (ax_off * x),      ay + (ay_off * y),
                           ax + (ax_off * x) + 32, ay + (ay_off * y) + 32);
            runFrame.scale(2.0f);
            float fs = frameSpeed;
            // Make last and first shorter
            if (x == 0 || x == 2) fs = frameSpeed / 2.0f;
            m_animation.addFrame(runFrame, fs);
        }
    }

    void onEnter(GameObject * owner, int /*prevStateId*/) override
    {
        Hero *hero = static_cast<Hero *>(owner);
        setFacingRight(hero->isFacingRight());
    }

    void update(Engine * /*e*/, float elapsedTime) override
    {
        m_animation.update(elapsedTime);
        
        Hero *hero = static_cast<Hero *>(m_stateMachine.owner());
        const b2Vec2& vel = hero->body()->GetLinearVelocity();

        if (std::abs(vel.x) > 0.01f) {
            bool fr = (vel.x >= 0.0f);
            hero->setFacingRight(fr);
            setFacingRight(fr);
        }

        if (std::abs(vel.y) > 0.1f) {
            m_stateMachine.changeState(FALL);
            return;
        }

        if (std::abs(vel.x) < 0.5f) {
            m_stateMachine.changeState(STAND);
            return;
        }
    }

    void draw(Engine *e) override
    {
        const b2Vec2& pos = m_stateMachine.owner()->body()->GetPosition();
        e->drawImage(m_animation.currentFrame(), pos.x, pos.y + 0.5f);
    }

    void setFacingRight(bool right) 
    {
        static bool animFacingRight = true;
        if (animFacingRight != right) {
            m_animation.flipVertically();
            animFacingRight = right;
        }
    }

    Animation m_animation;
};

//==============================================================================

Hero::Hero(Game *game, float x, float y, float w, float h)
    : GameObject(game)
    , m_jumpTimeout(0.0f)
    , m_facingRight(true)
    , m_feetContacts(0)
    , m_stateMachine(nullptr, 0)
{
    m_stateMachine.setOwner(this);

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
    
    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(hw, hh);
    
    b2FixtureDef fixtureDef;
    fixtureDef.filter.categoryBits = category();
    fixtureDef.filter.maskBits = GROUND | BOX | SENSOR;
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = 0.8f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.0f;

    // Main fixture
    body->CreateFixture(&fixtureDef);

    // Add foot sensor fixture
    polygonShape.SetAsBox(hw, 0.3f, b2Vec2(0, -hh), 0);
    fixtureDef.isSensor = true;
    fixtureDef.filter.maskBits = GROUND | BOX;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.0f;
    b2Fixture* footSensorFixture = body->CreateFixture(&fixtureDef);
    footSensorFixture->SetUserData(reinterpret_cast<void *>(FEET_SENSOR));

    m_body = body;

    // States
    GameObjectState *state = new StandHeroState(m_stateMachine);
    m_states.push_back(state);
    m_stateMachine.registerState(STAND, state);

    state = new RunHeroState(m_stateMachine);
    m_states.push_back(state);
    m_stateMachine.registerState(RUN, state);

    state = new FallHeroState(m_stateMachine);
    m_states.push_back(state);
    m_stateMachine.registerState(FALL, state);

    m_stateMachine.changeState(FALL);
}

//------------------------------------------------------------------------------

Hero::~Hero()
{
    for (auto *state : m_states)
        delete state;
}

//------------------------------------------------------------------------------

void Hero::update(Engine *e, float elapsedTime)
{
    const b2Vec2& centerOfMass = m_body->GetWorldCenter(); 

    if (e->isPressed(SDLK_RIGHT)) {
        m_body->ApplyForceToCenter(b2Vec2(10.0f, 0.0f));
    }

    if (e->isPressed(SDLK_LEFT)) {
        m_body->ApplyForceToCenter(b2Vec2(-10.0f, 0.0f));
    }

    if (e->isPressed(SDLK_UP)) {
        if (isOnGround() && m_jumpTimeout <= 0.0f) {
            m_body->ApplyLinearImpulse(b2Vec2(0.0f, 5.0f), centerOfMass);
            m_jumpTimeout = JUMP_DELAY;
        }
    } 

    // States
    //const b2Vec2& vel = m_stateMachine.owner()->body()->GetLinearVelocity();
    m_stateMachine.currentState()->update(e, elapsedTime);

    // Center view on player
    centerViewOn(e, centerOfMass.x, centerOfMass.y);

    if (m_jumpTimeout > 0.0f)
        m_jumpTimeout -= elapsedTime;
}

//------------------------------------------------------------------------------

void Hero::centerViewOn(Engine *e, float x, float y) const
{
    // Respect map borders
    float bLeft, bRight, bTop, bBottom;
    e->viewBounds(&bLeft, &bRight, &bBottom, &bTop);
    float hw = (bRight - bLeft) / 2.0f;
    float hh = (bTop - bBottom) / 2.0f;
    if (x < hw) x = hw;
    if (y < hh) y = hh;
    if (x > m_game->map()->width() - hw) x = m_game->map()->width() - hw;

    e->centerViewOn(x, y);
}

//------------------------------------------------------------------------------

void Hero::draw(Engine *e)
{
    m_stateMachine.currentState()->draw(e);
}

//------------------------------------------------------------------------------

void Hero::handleBeginContact(GameObject *other, void *fixtureUD)
{
    if (fixtureUD == (void*)FEET_SENSOR) {
        std::cout << "on ground" << std::endl;
        ++m_feetContacts;
    }

    if (other->category() != SENSOR) return;

    std::cout << "is touching" << std::endl;
}

//------------------------------------------------------------------------------

void Hero::handleEndContact(GameObject *other, void *fixtureUD)
{
    if (fixtureUD == (void*)FEET_SENSOR) {
        std::cout << "off ground" << std::endl;
        --m_feetContacts;
    }

    if (other->category() != SENSOR) return;

    std::cout << "is not touching" << std::endl;
}
