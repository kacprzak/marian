/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef EVENT_H
#define EVENT_H

#include "EventType.h"
#include "actors/Actor.h"
#include "components/PhysicsComponent.h"
#include "network/BaseSocketMgr.h"

#include <iostream>
#include <memory>

namespace event {

/*!
 * \brief The Event interface.
 */
class Event
{
 public:
    virtual ~Event() {}

    virtual EventType eventType() const = 0;
    virtual const char *eventName() const = 0;
    virtual void serialize(std::ostream& out) const = 0;
    virtual void deserialize(std::istream& in) = 0;
};

//typedef std::shared_ptr<Event> EventPtr;

//------------------------------------------------------------------------------

class BaseEvent : public Event
{
 protected:
    BaseEvent(EventType type, const char *name)
        : m_type(type)
        , m_name(name)
    {}

 public:
    EventType eventType() const override final { return m_type; }
    const char *eventName() const override final { return m_name; }
    void serialize(std::ostream& /*out*/) const override {}
    void deserialize(std::istream& /*in*/) override {}

 private:
    EventType m_type;
    const char* m_name;
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
        : BaseEvent(ACTOR_COLLIDED, "ActorCollided")
        , m_phase(phase)
        , m_actorA(actorA)
        , m_actorALimbData(actorALimbData)
        , m_actorB(actorB)
        , m_actorBLimbData(actorBLimbData)
    {}

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

//------------------------------------------------------------------------------

class MoveEvent : public BaseEvent
{
 public:
    MoveEvent(ActorId actor, float x, float y, float angle = 0.0f)
        : BaseEvent(ACTOR_MOVED, "ActorMoved")
        , m_actorId(actor)
        , m_x(x)
        , m_y(y)
        , m_angle(angle)
    {}

    MoveEvent(std::istream& in)
        : BaseEvent(ACTOR_MOVED, "ActorMoved")
    {
        deserialize(in);
    }

    void serialize(std::ostream& out) const
    {
        out << m_actorId << " "
            << m_x << " "
            << m_y << " "
            << m_angle;
    }

    void deserialize(std::istream& in)
    {
        in >> m_actorId;
        in >> m_x;
        in >> m_y;
        in >> m_angle;
    }

    ActorId m_actorId;
    float m_x;
    float m_y;
    float m_angle;
};

//------------------------------------------------------------------------------

class PhysicsStateChangeEvent : public BaseEvent
{
 public:
    PhysicsStateChangeEvent(ActorId actor, ActorPhysicsStateId newState)
        : BaseEvent(ACTOR_PHYSICS_STATE_CHANGED, "ActorPhysicsStateChanged")
        , m_actorId(actor)
        , m_newState(newState)
    {}

    PhysicsStateChangeEvent(std::istream& in)
        : BaseEvent(ACTOR_PHYSICS_STATE_CHANGED, "ActorPhysicsStateChanged")
    {
        deserialize(in);
    }

    void serialize(std::ostream& out) const
    {
        out << m_actorId << " "
            << static_cast<unsigned short>(m_newState);
    }

    void deserialize(std::istream& in)
    {
        unsigned short newStateVal;

        in >> m_actorId;
        in >> newStateVal;

        m_newState = ActorPhysicsStateId(newStateVal);
    }

    ActorId m_actorId;
    ActorPhysicsStateId m_newState;
};

//------------------------------------------------------------------------------

class ActorCreatedEvent : public BaseEvent
{
public:
    ActorCreatedEvent(ActorId actor, ActorCategory actorCategory,
                      float x = 0.0f, float y = 0.0f)
        : BaseEvent(ACTOR_CREATED, "ActorCreated")
        , m_actorId(actor)
        , m_actorCategory(actorCategory)
        , m_x(x)
        , m_y(y)
    {}

    ActorId m_actorId;
    ActorCategory m_actorCategory;
    float m_x;
    float m_y;
};

//------------------------------------------------------------------------------

class ActorDestroyedEvent : public BaseEvent
{
public:
    ActorDestroyedEvent(ActorId actor)
        : BaseEvent(ACTOR_DESTROYED, "ActorDestroyed")
        , m_actorId(actor)
    {}

    ActorId m_actorId;
};

//------------------------------------------------------------------------------

class ActorInputEvent : public BaseEvent
{
public:
    ActorInputEvent(ActorId actor, InputCommand command)
        : BaseEvent(INPUT_COMMAND, "ActorInput")
        , m_actorId(actor)
        , m_command(command)
    {}

    ActorInputEvent(std::istream& in)
        : BaseEvent(INPUT_COMMAND, "ActorInput")
    {
        deserialize(in);
    }

    void serialize(std::ostream& out) const
    {
        out << m_actorId << " "
            << static_cast<unsigned short>(m_command);
    }

    void deserialize(std::istream& in)
    {
        unsigned short newCommandVal;

        in >> m_actorId;
        in >> newCommandVal;

        m_command = InputCommand(newCommandVal);
    }

    ActorId m_actorId;
    InputCommand m_command;
};

//------------------------------------------------------------------------------

class RemoteClientEvent : public BaseEvent
{
public:
    RemoteClientEvent(int socketId, int ip)
        : BaseEvent(REMOTE_CLIENT, "RemoteClient")
        , m_socketId(socketId)
        , m_ip(ip)
    {}

    void serialize(std::ostream& out) const
    {
        out << "socketId: " << m_socketId
            << " ip: " << net::BaseSocketMgr::singleton().getHostByAddr(m_ip);
    }

    int m_socketId;
    int m_ip;
};

} // namespace event

#endif // EVENT_H
