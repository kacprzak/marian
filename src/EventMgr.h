/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <EventType.h>
#include "Event.h"
#include "Singleton.h"
#include <map>
#include <list>
#include <memory>

typedef std::function<void (EventPtr)> EventListener;
typedef std::shared_ptr<EventListener> EventListenerPtr;

class EventMgr : public Singleton<EventMgr> {

    typedef std::multimap<EventType, EventListenerPtr> EventListenerMap;
    typedef std::list<EventPtr> EventQueue;

 public:
    EventMgr();
    ~EventMgr();

    void addListener(EventType et, EventListenerPtr listener);
    void removeListener(EventType et, EventListenerPtr listener);

    void triggerEvent(const EventPtr& event);

    void queueEvent(const EventPtr& event);

    void update();

    // For testing
    int listenersCount() const { return m_listeners.size(); }

    static EventListenerPtr makeListener(EventListener el)
    {
        return EventListenerPtr(new EventListener(el));
    }

 private:
    static const int NUM_OF_QUEUES = 2;
    EventListenerMap m_listeners;
    EventQueue m_eventQueues[NUM_OF_QUEUES];
    int m_activeQueue;
};

#endif // EVENTMANAGER_H
