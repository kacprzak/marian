/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */

#include <EventType.h>
#include "Event.h"
#include "Singleton.h"
#include <map>
#include <list>

class EventManager : public Singleton<EventManager> {

    typedef std::function<void (EventPtr)> EventListener;
    typedef std::multimap<EventType, EventListener> EventListenerMap;
    typedef std::list<EventPtr> EventQueue;

 public:
    EventManager();

    void addListener(EventType et, EventListener listener);

    void triggerEvent(const EventPtr& event);

    void queueEvent(const EventPtr& event);

    void update();

 private:
    static const int NUM_OF_QUEUES = 2;
    EventListenerMap m_listeners;
    EventQueue m_eventQueues[NUM_OF_QUEUES];
    int m_activeQueue;
};
