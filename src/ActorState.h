/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef ACTORSTATE_H
#define ACTORSTATE_H

#include "StateMachine.h"

class Actor;
class ActorState;

typedef StateMachine<Actor *, ActorState> ActorStateMachine;

class ActorState : public State<Actor *>
{
 public:
    ActorState(ActorStateMachine& stateMachine)
        : State<Actor *>()
        , m_stateMachine(stateMachine)
    {}

    virtual void onEnter(Actor * /*owner*/, int /*prevStateId*/) override {}
    virtual void onExit(Actor * /*owner*/, int /*nextStateId*/) override {}

    virtual void update(Engine *e, float elapsedTime) = 0;
    virtual void draw(Engine *e) = 0;

 protected:
    ActorStateMachine& m_stateMachine;
};

#endif // ACTORSTATE_H
