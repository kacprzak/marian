/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "EventMgr.h"

#include "Logger.h"

using namespace event;

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

void EventMgr::addListener(EventType et, std::shared_ptr<EventListener> listener)
{
    // TODO: Prevent double registration

    m_listeners.insert(std::make_pair(et, listener));
}

//------------------------------------------------------------------------------

void EventMgr::removeListener(EventType et, std::shared_ptr<EventListener> listener)
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

void EventMgr::triggerEvent(std::unique_ptr<Event> event)
{
    auto ii = m_listeners.equal_range(event->eventType());
    for(auto i = ii.first; i != ii.second; ++i) {
        (*(i->second))(*event);
    }
}

//------------------------------------------------------------------------------

void EventMgr::queueEvent(std::unique_ptr<Event> event)
{
    // Check if the are listeners for event
    auto listenersIter = m_listeners.find(event->eventType());
    if (listenersIter != std::end(m_listeners)) {
        m_eventQueues[m_activeQueue].push_back(std::move(event));
    }
}

//------------------------------------------------------------------------------

void EventMgr::update()
{
    for (std::unique_ptr<Event>& event : m_eventQueues[m_activeQueue]) {
        triggerEvent(std::move(event));
    }
    m_eventQueues[m_activeQueue].clear();

    m_activeQueue = (m_activeQueue + 1) % NUM_OF_QUEUES;
}


