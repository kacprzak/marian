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
    RUN
};

//==============================================================================

class HeroState : public State<Hero *>
{
 public:
    HeroState(StateMachine<Hero *>& stateMachine)
        : State<Hero *>()
        , m_stateMachine(stateMachine)
    {}

    virtual void onEnter(Hero *owner, int prevStateId) override {}
    virtual void onExit(Hero *owner, int nextStateId) override {}

    virtual void update(Engine *e, float elapsedTime) = 0;
    virtual void draw(Engine *e) = 0;

    StateMachine<Hero *>& m_stateMachine;
};

//==============================================================================

class StandHeroState : public HeroState
{
 public:
    StandHeroState(StateMachine<Hero *>& stateMachine)
        : HeroState(stateMachine)
    {
        const Texture *tex = ResourceMgr::instance().getTexture("MegaMan_001.png");

        int ax = 38;
        int ax_off = 36;
        m_animation.setReversable(true);
        Image idleFrame1(tex, ax, 255, ax + 32, 255 + 32);
        idleFrame1.scale(2.0f);
        m_animation.addFrame(idleFrame1, 1.0f);
        Image idleFrame2(tex, ax + ax_off, 255, ax + ax_off + 32, 255 + 32);
        idleFrame2.scale(2.0f);
        m_animation.addFrame(idleFrame2, 0.1f); // Blink
    }

    void update(Engine * /*e*/, float elapsedTime) override
    {
        m_animation.update(elapsedTime);
    }

    void draw(Engine *e) override
    {
        const b2Vec2& pos = m_stateMachine.owner()->body()->GetPosition();
        e->drawImage(m_animation.currentFrame(), pos.x, pos.y + 0.5f);
    }

    Animation m_animation;
};

//==============================================================================

class RunHeroState : public HeroState
{
 public:
    RunHeroState(StateMachine<Hero *>& stateMachine)
        : HeroState(stateMachine)
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
    }

    void draw(Engine *e) override
    {
        const b2Vec2& pos = m_stateMachine.owner()->body()->GetPosition();
        e->drawImage(m_animation.currentFrame(), pos.x, pos.y + 0.5f);
    }

    Animation m_animation;
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
    State<Hero *> *state = new StandHeroState(m_stateMachine);
    m_states.push_back(state);
    m_stateMachine.registerState(STAND, state);

    state = new RunHeroState(m_stateMachine);
    m_states.push_back(state);
    m_stateMachine.registerState(RUN, state);

    m_stateMachine.changeState(RUN);
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
    static_cast<HeroState *>(m_stateMachine.currentState())->update(e, elapsedTime);

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
    static_cast<HeroState *>(m_stateMachine.currentState())->draw(e);
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
