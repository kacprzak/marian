/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef HERO_H
#define HERO_H

#include "Actor.h"
#include "Map.h"
#include "ActorState.h"

#include <memory>

class Engine;

class Hero : public Actor
{
 public:
    Hero(unsigned long id, Game *game, float x, float y, float w, float h);
    ~Hero();
  
    void update(Engine *e, float elapsedTime) override;
    void draw(Engine *e) override;

    ActorCategory category() override
    { return HERO; }

    void handleBeginContact(Actor *other, void *fixtureUD = nullptr) override;
    void handleEndContact  (Actor *other, void *fixtureUD = nullptr) override;

    bool isOnGround() const { return m_feetContacts > 0; }
    void setFacingRight(bool right) { m_facingRight = right; }
    bool isFacingRight() const { return m_facingRight; }

 private:
    void centerViewOn(Engine *e, float x, float y) const;

    float m_jumpTimeout;
    bool m_facingRight;
    int m_feetContacts;

    ActorStateMachine m_stateMachine;
    std::vector<ActorState *> m_states;
};

#endif
