/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */

#include "EventMgr.h"

#include "Logger.h"

EventMgr::EventMgr()
    : m_activeQueue(0)
{
    LOG << "created EventMgr\n";
}

//------------------------------------------------------------------------------

EventMgr::~EventMgr()
{
    LOG << "destroyed EventMgr\n";
}

//------------------------------------------------------------------------------

void EventMgr::addListener(EventType et, EventListenerPtr listener)
{
    // TODO: Prevent double registration

    m_listeners.insert(std::make_pair(et, listener));
}

//------------------------------------------------------------------------------

void EventMgr::removeListener(EventType et, EventListenerPtr listener)
{
    auto ii = m_listeners.equal_range(et);
    for(auto i = ii.first; i != ii.second;) {
        if (i->second == listener) {
            i = m_listeners.erase(i);
        } else {
            ++i;
        }
    }
}

//------------------------------------------------------------------------------

void EventMgr::triggerEvent(const EventPtr& event)
{
    auto ii = m_listeners.equal_range(event->eventType());
    for(auto i = ii.first; i != ii.second; ++i) {
        (*(i->second))(event);
    }
}

//------------------------------------------------------------------------------

void EventMgr::queueEvent(const EventPtr& event)
{
    // Check if the are listeners for event
    auto listenersIter = m_listeners.find(event->eventType());
    if (listenersIter != std::end(m_listeners)) {
        m_eventQueues[m_activeQueue].push_back(event);
    }
}

//------------------------------------------------------------------------------

void EventMgr::update()
{
    for (EventPtr event : m_eventQueues[m_activeQueue]) {
        triggerEvent(event);
    }
    m_eventQueues[m_activeQueue].clear();

    m_activeQueue = (m_activeQueue + 1) % NUM_OF_QUEUES;
}


