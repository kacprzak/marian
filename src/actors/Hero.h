/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef HERO_H
#define HERO_H

#include "Actor.h"
#include "components/PhysicsComponent.h"
#include "components/RenderComponent.h"
#include "StateMachine.h"

class Engine;

class HeroRenderComponent;
class HeroRenderComponentState;

typedef StateMachine<HeroRenderComponent *, HeroRenderComponentState> HeroRenderComponentStateMachine;

class HeroRenderComponent : public RenderComponent
{
 public:
    HeroRenderComponent();
    ~HeroRenderComponent();

    bool init() override;

    const Image& currentImage() const override;
    float yOffset() const override { return 0.5f; }

    void changePhysicsState(ActorPhysicsStateId newState);

    void update(Engine *e, float elapsedTime) override;

    bool isFacingRight() const { return m_facingRight; }

 private:
    void setFacingRight(bool right);

    bool m_facingRight;
    float m_jumpTimeout;

    HeroRenderComponentStateMachine m_stateMachine;
    std::vector<HeroRenderComponentState *> m_states;
};


class HeroPhysicsComponent : public PhysicsComponent
{
 public:
    HeroPhysicsComponent(Game *game, float x, float y,
                        float w = 1.0f, float h = 1.0f);

    void handleBeginContact(Actor *other, void *fixtureUD = nullptr) override;
    void handleEndContact  (Actor *other, void *fixtureUD = nullptr) override;

    void update(Engine *e, float elapsedTime) override;

    bool isOnGround() const { return m_feetContacts > 0; }

    void changeState(ActorPhysicsStateId state);

 private:
    int m_feetContacts;
    int m_heroStateId;

    float m_lastX;
    float m_lastY;
};

#endif
