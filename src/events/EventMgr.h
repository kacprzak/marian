#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Event.h"
#include "EventType.h"
#include "Singleton.h"

#include <functional>
#include <list>
#include <map>
#include <memory>

namespace event {

using EventListener = std::function<void(Event&)>;

/*!
 * \brief The global event manager class.
 */
class EventMgr : public Singleton<EventMgr>
{
    using EventListenerMap =
        std::multimap<EventType, std::shared_ptr<EventListener>>;
    using EventQueue = std::list<std::unique_ptr<Event>>;

  public:
    EventMgr();
    ~EventMgr() override;

    // todo: Maybe they should be weak_ptr's
    void addListener(EventType et, std::shared_ptr<EventListener> listener);
    void removeListener(EventType et, std::shared_ptr<EventListener> listener);

    void triggerEvent(std::unique_ptr<Event> event);
    void queueEvent(std::unique_ptr<Event> event);

    void update();

    // For testing
    int listenersCount() const { return m_listeners.size(); }

  private:
    static const int NUM_OF_QUEUES = 2;
    EventListenerMap m_listeners;
    EventQueue m_eventQueues[NUM_OF_QUEUES];
    int m_activeQueue;
};

//==============================================================================

/*!
 * \brief Useful for classes that want to register methods as event listeners.
 */
class EventListenerHelper final
{
  public:
    ~EventListenerHelper() { unregisterAll(); }

    bool registerListener(EventType type, EventListener listener)
    {
        std::shared_ptr<EventListener> el(new EventListener(listener));
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
    std::multimap<EventType, std::shared_ptr<EventListener>> m_listeners;
};

} // namespace event

#endif // EVENTMANAGER_H
