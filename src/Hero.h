/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef HERO_H
#define HERO_H

#include "GameObject.h"

#include "Map.h"
#include "GameObjectState.h"
#include <memory>

class Engine;

class Hero : public GameObject
{
 public:
    Hero(Game *game, float x, float y, float w, float h);
    ~Hero();
  
    void update(Engine *e, float elapsedTime) override;
    void draw(Engine *e) override;

    GameObjectCategory category() override
    { return HERO; }

    void handleBeginContact(GameObject *other, void *fixtureUD = nullptr) override;
    void handleEndContact  (GameObject *other, void *fixtureUD = nullptr) override;

    bool isOnGround() const { return m_feetContacts > 0; }
    void setFacingRight(bool right) { m_facingRight = right; }
    bool isFacingRight() const { return m_facingRight; }

 private:
    void centerViewOn(Engine *e, float x, float y) const;

    float m_jumpTimeout;
    bool m_facingRight;
    int m_feetContacts;

    GameObjectStateMachine m_stateMachine;
    std::vector<GameObjectState *> m_states;
};

#endif
