/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef EVENT_H
#define EVENT_H

#include <EventType.h>
#include <iostream>
#include <memory>

class Event
{

 public:
    virtual EventType eventType() const = 0;
    virtual const char *eventName() const = 0;
    virtual void serialize(std::ostream& out) const = 0;
};

typedef std::shared_ptr<Event> EventPtr;

class BaseEvent : public Event
{
 public:
    BaseEvent(EventType type)
        : m_type(type)
    {}

    EventType eventType() const override { return m_type; }
    const char *eventName() const override { return "BaseEvent"; }
    void serialize(std::ostream& out) const override {}

 private:
    EventType m_type;
};

#endif // EVENT_H
