#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE EventManagerTest
#include <boost/test/unit_test.hpp>
#include "EventMgr.h"


void eventListener(EventPtr event)
{
    std::clog << "EVENT: " << event->eventName() << " {";
    event->serialize(std::clog);
    std::clog << " }"<< std::endl;
}


BOOST_AUTO_TEST_CASE(eventmanager_test)
{
    new EventMgr;

    EventMgr *em = nullptr;
    em = EventMgr::singletonPtr();

    BOOST_REQUIRE( em );

    BOOST_REQUIRE( em->listenersCount() == 0 );

    EventListenerPtr el = EventMgr::makeListener(eventListener);
    em->addListener(ACTOR_COLLIDED, el);

    BOOST_REQUIRE( em->listenersCount() == 1 );

    em->removeListener(ACTOR_COLLIDED, el);

    BOOST_REQUIRE( em->listenersCount() == 0 );

    delete EventMgr::singletonPtr();
}
