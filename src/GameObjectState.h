/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAMEOBJECTSTATE_H
#define GAMEOBJECTSTATE_H

#include "StateMachine.h"

class GameObject;
class GameObjectState;

typedef StateMachine<GameObject *, GameObjectState> GameObjectStateMachine;

class GameObjectState : public State<GameObject *>
{
 public:
    GameObjectState(GameObjectStateMachine& stateMachine)
        : State<GameObject *>()
        , m_stateMachine(stateMachine)
    {}

    virtual void onEnter(GameObject * /*owner*/, int /*prevStateId*/) override {}
    virtual void onExit(GameObject * /*owner*/, int /*nextStateId*/) override {}

    virtual void update(Engine *e, float elapsedTime) = 0;
    virtual void draw(Engine *e) = 0;

 protected:
    GameObjectStateMachine& m_stateMachine;
};

#endif // GAMEOBJECTSTATE_H
