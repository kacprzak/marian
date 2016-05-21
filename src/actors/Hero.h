/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef HERO_H
#define HERO_H

#include "Actor.h"
#include "components/PhysicsComponent.h"
#include "components/RenderComponent.h"
#include "StateMachine.h"

class Engine;

class HeroRenderComponent : public RenderComponent
{
 public:
    HeroRenderComponent() {}

};


class HeroPhysicsComponent : public PhysicsComponent
{
 public:
    HeroPhysicsComponent(GameLogic *game, float x, float y,
                         float w = 1.0f, float h = 1.0f);

    void handleBeginContact(ActorPtr other, void *fixtureUD = nullptr) override;
    void handleEndContact  (ActorPtr other, void *fixtureUD = nullptr) override;

    void update(float elapsedTime) override;

    bool isOnGround() const { return m_feetContacts > 0; }

    void changeState(ActorPhysicsStateId state);

    void handleInputCommand(event::InputCommand command);

 private:
    int m_feetContacts;
    int m_heroStateId;

    float m_jumpTimeout;

    float m_lastX;
    float m_lastY;

    bool m_movingRight;
    bool m_movingLeft;
    bool m_jumping;
};

#endif
