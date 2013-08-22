/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef HERO_H
#define HERO_H

#include "Actor.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "AiComponent.h"
#include "ActorState.h"

class Engine;

class HeroAiComponent : public AiComponent
{
 public:
    void update(Engine *e, float elapsedTime) override;

 private:
    float m_jumpTimeout;
};


class HeroRenderComponent : public RenderComponent
{
 public:
    HeroRenderComponent();
    ~HeroRenderComponent();

    bool init() override;

    const Image& currentImage() const override;
    float yOffset() const override { return 0.5f; }

    void update(Engine *e, float elapsedTime) override;

    bool isFacingRight() const { return m_facingRight; }

 private:
    void setFacingRight(bool right) { m_facingRight = right; }

    bool m_facingRight;
    float m_jumpTimeout;

    ActorStateMachine m_stateMachine;
    std::vector<ActorState *> m_states;
};


class HeroPhysicsComponent : public PhysicsComponent
{
 public:
    HeroPhysicsComponent(Game *game, float x, float y,
                        float w = 1.0f, float h = 1.0f);

    void handleBeginContact(Actor *other, void *fixtureUD = nullptr) override;
    void handleEndContact  (Actor *other, void *fixtureUD = nullptr) override;

    bool isOnGround() const { return m_feetContacts > 0; }

 private:
    int m_feetContacts;
};

#endif
