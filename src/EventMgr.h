/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "EventType.h"
#include "Event.h"
#include "Singleton.h"
#include <map>
#include <list>
#include <memory>

typedef std::function<void (EventPtr)> EventListener;
typedef std::shared_ptr<EventListener> EventListenerPtr;

class EventMgr : public Singleton<EventMgr>
{
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

//==============================================================================

/**
 * Useful for classes that want to register methods as event listeners.
 */
class EventListenerHelper final
{
public:

    ~EventListenerHelper()
    {
        unregisterAll();
    }

    bool registerListener(EventType type, EventListener listener)
    {
        EventListenerPtr el(new EventListener(listener));
        EventMgr::singleton().addListener(type, el);
        m_listeners.insert(std::make_pair(type, el));

        return true;
    }

    void unregisterAll()
    {
        for (auto pair : m_listeners) {
            EventMgr::singleton().removeListener(pair.first, pair.second);
        }
    }

 private:
    // Keeps pointers to make unregistration possible
    std::multimap<EventType, EventListenerPtr> m_listeners;
};

#endif // EVENTMANAGER_H
