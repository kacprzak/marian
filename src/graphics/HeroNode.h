/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef HERONODE_H
#define HERONODE_H

#include "SpriteNode.h"
#include "StateMachine.h"
#include "components/PhysicsComponent.h"

#include <vector>

namespace gfx {

class HeroNode;
class HeroNodeState;

using HeroNodeStateMachine = StateMachine<HeroNode *, HeroNodeState>;

class HeroNode : public SpriteNode
{
public:
    HeroNode();
    ~HeroNode() override;

    void update(float elapsedTime) override;
    void moveTo(float x, float y, float angle) override;

    void changePhysicsState(ActorPhysicsStateId newState) override;

    void flipHorizontally(bool flip) override { setFacingRight(!flip); }

    bool isFacingRight() const { return m_facingRight; }

 private:
    void setFacingRight(bool right);

    bool m_facingRight;

    HeroNodeStateMachine m_stateMachine;
    std::vector<HeroNodeState *> m_states;
};

} // namespace gfx

#endif // HERONODE_H
