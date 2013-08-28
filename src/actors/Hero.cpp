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

//==============================================================================

class HeroRenderComponentState : public State<HeroRenderComponent *>
{
 public:
    HeroRenderComponentState(HeroRenderComponentStateMachine& stateMachine)
        : State<HeroRenderComponent *>()
        , m_stateMachine(stateMachine)
    {}

    void onEnter(HeroRenderComponent * owner, int /*prevStateId*/) override
    {
        setFacingRight(owner->isFacingRight());
    }

    virtual void onExit(HeroRenderComponent * /*owner*/, int /*nextStateId*/) override {}

    virtual void update(Engine *e, float elapsedTime) = 0;

    virtual const Image& currentImage() const = 0;

    virtual void setFacingRight(bool right) = 0;

 protected:
    HeroRenderComponentStateMachine& m_stateMachine;
};

//==============================================================================

class StandHeroState : public HeroRenderComponentState
{
 public:
    StandHeroState(HeroRenderComponentStateMachine& stateMachine)
        : HeroRenderComponentState(stateMachine)
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

    void update(Engine * /*e*/, float elapsedTime) override
    {
        m_animation.update(elapsedTime);
    }

    const Image& currentImage() const override
    {
        return m_animation.currentFrame();
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

class FallHeroState : public HeroRenderComponentState
{
 public:
    FallHeroState(HeroRenderComponentStateMachine& stateMachine)
        : HeroRenderComponentState(stateMachine)
    {
        const Texture *tex = ResourceMgr::singleton().getTexture("MegaMan_001.png");

        int ax = 38;
        int ax_off = 4*36;
        int ay = 219;

        m_image = std::unique_ptr<Image>(new Image(tex, ax + ax_off, ay, ax + ax_off + 32, ay + 32));
        m_image->scale(2.0f);
    }

    void update(Engine * /*e*/, float /*elapsedTime*/) override
    {
        // Do nothing
    }

    const Image& currentImage() const override
    {
        return *m_image;
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

class RunHeroState : public HeroRenderComponentState
{
 public:
    RunHeroState(HeroRenderComponentStateMachine& stateMachine)
        : HeroRenderComponentState(stateMachine)
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

    void update(Engine * /*e*/, float elapsedTime) override
    {
        m_animation.update(elapsedTime);
    }

    const Image& currentImage() const override
    {
        return m_animation.currentFrame();
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

HeroRenderComponent::HeroRenderComponent()
    : m_facingRight(true)
    , m_jumpTimeout(0.0f)
    , m_stateMachine(nullptr, 0)
{
}

//------------------------------------------------------------------------------

HeroRenderComponent::~HeroRenderComponent()
{
    for (auto *state : m_states)
        delete state;
}

//------------------------------------------------------------------------------

bool HeroRenderComponent::init()
{
    m_stateMachine.setOwner(this);

    // States
    HeroRenderComponentState *state = new StandHeroState(m_stateMachine);
    m_states.push_back(state);
    m_stateMachine.registerState(STAND, state);

    state = new RunHeroState(m_stateMachine);
    m_states.push_back(state);
    m_stateMachine.registerState(RUN, state);

    state = new FallHeroState(m_stateMachine);
    m_states.push_back(state);
    m_stateMachine.registerState(FALL, state);

    m_stateMachine.changeState(STAND);

    return true;
}

//------------------------------------------------------------------------------

const Image& HeroRenderComponent::currentImage() const
{
    const HeroRenderComponentState *renderState = static_cast<const HeroRenderComponentState*>(m_stateMachine.currentState());
    return renderState->currentImage();
}

//------------------------------------------------------------------------------

void HeroRenderComponent::changePhysicsState(ActorPhysicsStateId newState)
{
    m_stateMachine.changeState(newState);
}

//------------------------------------------------------------------------------

void HeroRenderComponent::update(float elapsedTime)
{
    auto whpc = m_owner->getComponent<HeroPhysicsComponent>(PHYSICS);
    Engine& e = Engine::singleton();

    if (auto shpc = whpc.lock()) {

        if (e.isPressed(SDL_SCANCODE_RIGHT)) {
            shpc->applyForceToCenter(10.0f, 0.0f);
            setFacingRight(true);
        }

        if (e.isPressed(SDL_SCANCODE_LEFT)) {
            shpc->applyForceToCenter(-10.0f, 0.0f);
            setFacingRight(false);
        }

        if (e.isPressed(SDL_SCANCODE_UP)) {
            if (shpc->isOnGround() && m_jumpTimeout <= 0.0f) {
                shpc->applyLinearImpulse(0.0f, 5.0f);
                m_jumpTimeout = JUMP_DELAY;
            }
        }
    }

    // States
    m_stateMachine.currentState()->update(&e, elapsedTime);

    if (m_jumpTimeout > 0.0f)
        m_jumpTimeout -= elapsedTime;
}

//------------------------------------------------------------------------------

void HeroRenderComponent::setFacingRight(bool right)
{
    m_facingRight = right;
    m_stateMachine.currentState()->setFacingRight(right);
}

//==============================================================================

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

    m_lastX = posX();
    m_lastY = posY();
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

void HeroPhysicsComponent::update(float /*elapsedTime*/)
{
    switch(m_heroStateId) {
    case FALL:
    {
        if (isOnGround() && std::abs(velY()) < 0.1f) {
            changeState(STAND);
            return;
        }
        break;
    }

    case RUN:
    {
        if (std::abs(velY()) > 0.1f) {
            changeState(FALL);
            return;
        }

        if (std::abs(velX()) < 0.5f) {
            changeState(STAND);
            return;
        }
        break;
    }

    case STAND:
    {
        if (std::abs(velY()) > 6.0f) {
            changeState(FALL);
            return;
        }

        if (std::abs(velX()) > 0.6f) {
            changeState(RUN);
            return;
        }
        break;
    }
    }

    // emit move event if position changed
    if (m_lastX != posX() || m_lastY != posY()) {
        EventMgr::singleton().queueEvent(EventPtr(new MoveEvent(m_owner->id(), posX(), posY())));
    }

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

