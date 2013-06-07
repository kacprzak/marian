/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */

#include "EventManager.h"

EventManager::EventManager()
    : m_activeQueue(0)
{
}

//------------------------------------------------------------------------------

void EventManager::addListener(EventType et, EventListener listener)
{
    m_listeners.insert(std::make_pair(et, listener));
}

//------------------------------------------------------------------------------

void EventManager::triggerEvent(const EventPtr& event)
{
    auto ii = m_listeners.equal_range(event->eventType());
    for(auto i = ii.first; i != ii.second; ++i) {
        i->second(event);
    }
}

//------------------------------------------------------------------------------

void EventManager::queueEvent(const EventPtr& event)
{
    // Check if the are listeners for event
    auto listenersIter = m_listeners.find(event->eventType());
    if (listenersIter != std::end(m_listeners)) {
        m_eventQueues[m_activeQueue].push_back(event);
    }
}

//------------------------------------------------------------------------------

void EventManager::update()
{
    for (EventPtr event : m_eventQueues[m_activeQueue]) {
        triggerEvent(event);
    }

    m_activeQueue = ++m_activeQueue % NUM_OF_QUEUES;
}


