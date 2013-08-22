/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef EVENT_H
#define EVENT_H

#include <EventType.h>
#include <iostream>
#include <memory>

#include "Actor.h"

class Event
{

 public:
    virtual EventType eventType() const = 0;
    virtual const char *eventName() const = 0;
    virtual void serialize(std::ostream& out) const = 0;
};

typedef std::shared_ptr<Event> EventPtr;

//------------------------------------------------------------------------------

class BaseEvent : public Event
{
 public:
    BaseEvent(EventType type)
        : m_type(type)
    {}

    EventType eventType() const override { return m_type; }
    const char *eventName() const override { return "BaseEvent"; }
    void serialize(std::ostream& /*out*/) const override {}

 private:
    EventType m_type;
};

//------------------------------------------------------------------------------

class CollisionEvent : public BaseEvent
{
 public:
    enum Phase {
        BEGIN, END
    };

    CollisionEvent(Phase phase, ActorId actorA, void *actorALimbData,
                   ActorId actorB, void *actorBLimbData)
        : BaseEvent(ACTOR_COLLIDED)
        , m_phase(phase)
        , m_actorA(actorA)
        , m_actorALimbData(actorALimbData)
        , m_actorB(actorB)
        , m_actorBLimbData(actorBLimbData)
    {}

    const char *eventName() const override { return "ActorCollided"; }
    
    void serialize(std::ostream& out) const override
    {
        const char *phase = (m_phase == BEGIN) ? "BEGIN" : "END  ";
        out << phase << " A id: " << m_actorA << " B id: " << m_actorB
            << " A limb: " << m_actorALimbData << " B limb: " << m_actorBLimbData;
    }

 //private:
    Phase m_phase;

    ActorId m_actorA;
    void *m_actorALimbData;
    ActorId m_actorB;
    void *m_actorBLimbData;
};

#endif // EVENT_H
