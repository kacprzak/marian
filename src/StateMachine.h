/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <map>
#include <cassert>

template <typename T>
class State
{
 public:
    virtual ~State() {}

    virtual void onEnter(T *owner, int prevStateId) = 0;
    virtual void onExit(T *owner, int nextStateId) = 0;
};

/**
 * Simple state machine. It does not own registered states.
 */
template <typename T>
class StateMachine
{
 public:
    StateMachine(T *owner, int idleStateId)
        : m_owner(owner)
        , m_idleStateId(idleStateId)
        , m_currentStateId(idleStateId)
        , m_currentState(nullptr)
    {}

    T *owner() { return m_owner; }
    const T *owner() const { return m_owner; }
    void setOwner(T *owner) { m_owner = owner; }

    State<T> *state(int stateId) { return m_states[stateId]; }

    const int *stateId(State<T> *state)
    {
        for (const auto& kv : m_states) {
            if (kv.second == state)
                return &kv.first;
        }
        return nullptr;
    }

    void registerState(int stateId, State<T> *state)
    {
        assert(state);
        assert(!registered(stateId));
        assert(!registered(state));

        m_states[stateId] = state;
    }
    
    bool registered(int stateId) const
    {
        return m_states.find(stateId) != end(m_states);
    }

    bool registered(State<T> *state) const
    {
        return stateId(state) != nullptr;
    }

    void changeState(int stateId)
    {
        assert(stateId != m_idleStateId);

        if (m_currentState)
            m_currentState->onExit(m_owner, stateId);

        auto *nextState = state(stateId);
        assert(nextState);

        int prevStateId = m_currentStateId;

        m_currentStateId = stateId;
        m_currentState = nextState;

        nextState->onExit(m_owner, prevStateId);
    }

 private:
    T *m_owner;
    int m_idleStateId;
    int m_currentStateId;
    State<T> *m_currentState;
    std::map<int, State<T> *> m_states;

};

#endif // STATEMACHINE_H
