#ifndef HERONODE_H
#define HERONODE_H

#include "SpriteNode.h"
#include "StateMachine.h"
#include "components/PhysicsComponent.h"
#include <vector>

class HeroNode;
class HeroNodeState;

typedef StateMachine<HeroNode *, HeroNodeState> HeroNodeStateMachine;

class HeroNode : public SpriteNode
{
public:
    HeroNode();
    ~HeroNode();

    void update(float elapsedTime) override;
    void moveTo(float x, float y, float angle);

    void changePhysicsState(ActorPhysicsStateId newState) override;

    bool isFacingRight() const { return m_facingRight; }

 private:
    void setFacingRight(bool right);

    bool m_facingRight;

    HeroNodeStateMachine m_stateMachine;
    std::vector<HeroNodeState *> m_states;
};

#endif // HERONODE_H
