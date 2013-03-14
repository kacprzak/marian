/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Hero.h"

#include "Engine.h"
#include "ResourceMgr.h"
#include "Game.h"
#include "Animation.h"
#include <iostream>
#include <Box2D/Box2D.h>

#define JUMP_DELAY 1.0f

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
        , m_runsRight(true)
    {
        const Texture *tex = ResourceMgr::instance().getTexture("MegaMan_001.png");

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

    void update(Engine * /*e*/, float elapsedTime) override
    {
        m_animation.update(elapsedTime);

        const b2Vec2& vel = m_stateMachine.owner()->body()->GetLinearVelocity();

        if (vel.x < 0.05f && m_runsRight) {
            m_animation.flipVertically();
            m_runsRight = false;
        }

        if (vel.x > 0.05f && !m_runsRight) {
            m_animation.flipVertically();
            m_runsRight = true;
        }
 
        if (std::abs(vel.y) > 0.2f) {
            m_stateMachine.changeState(FALL);
            return;
        }

        if (std::abs(vel.x) > 0.2f) {
            m_stateMachine.changeState(RUN);
            return;
        }
    }

    void draw(Engine *e) override
    {
        const b2Vec2& pos = m_stateMachine.owner()->body()->GetPosition();
        e->drawImage(m_animation.currentFrame(), pos.x, pos.y + 0.5f);
    }

    Animation m_animation;
    bool m_runsRight;
};

//==============================================================================

class FallHeroState : public GameObjectState
{
 public:
    FallHeroState(GameObjectStateMachine& stateMachine)
        : GameObjectState(stateMachine)
        , m_runsRight(true)
    {
        const Texture *tex = ResourceMgr::instance().getTexture("MegaMan_001.png");

        int ax = 38;
        int ax_off = 4*36;
        int ay = 219;

        m_image = std::unique_ptr<Image>(new Image(tex, ax + ax_off, ay, ax + ax_off + 32, ay + 32));
        m_image->scale(2.0f);
    }

    void update(Engine * /*e*/, float elapsedTime) override
    {
        const b2Vec2& vel = m_stateMachine.owner()->body()->GetLinearVelocity();

        if (std::abs(vel.y) < 0.1f) {
            m_stateMachine.changeState(STAND);
            return;
        }

        if (vel.x < 0.02f && m_runsRight) {
            m_image->flipVertically();
            m_runsRight = false;
        }

        if (vel.x > 0.02f && !m_runsRight) {
            m_image->flipVertically();
            m_runsRight = true;
        } 
    }

    void draw(Engine *e) override
    {
        const b2Vec2& pos = m_stateMachine.owner()->body()->GetPosition();
        e->drawImage(*m_image, pos.x, pos.y + 0.5f);
    }

    std::unique_ptr<Image> m_image;
    bool m_runsRight;
};

//==============================================================================

class RunHeroState : public GameObjectState
{
 public:
    RunHeroState(GameObjectStateMachine& stateMachine)
        : GameObjectState(stateMachine)
        , m_runsRight(true)
    {
        const Texture *tex = ResourceMgr::instance().getTexture("MegaMan_001.png");

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

    void update(Engine * /*e*/, float elapsedTime) override
    {
        m_animation.update(elapsedTime);

        const b2Vec2& vel = m_stateMachine.owner()->body()->GetLinearVelocity();

        if (std::abs(vel.y) > 0.1f) {
            m_stateMachine.changeState(FALL);
            return;
        }

        if (std::abs(vel.x) < 0.1f) {
            m_stateMachine.changeState(STAND);
            return;
        }

        if (vel.x < 0 && m_runsRight) {
            m_animation.flipVertically();
            m_runsRight = false;
        }

        if (vel.x > 0 && !m_runsRight) {
            m_animation.flipVertically();
            m_runsRight = true;
        } 
    }

    void draw(Engine *e) override
    {
        const b2Vec2& pos = m_stateMachine.owner()->body()->GetPosition();
        e->drawImage(m_animation.currentFrame(), pos.x, pos.y + 0.5f);
    }

    Animation m_animation;
    bool m_runsRight;
};

//==============================================================================

Hero::Hero(Game *game, float x, float y, float w, float h)
    : GameObject(game)
    , m_jumpTimeout(0.0f)
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
    m_stateMachine.currentState()->update(e, elapsedTime);

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
    m_stateMachine.currentState()->draw(e);
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
