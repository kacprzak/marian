/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE EventManagerTest
#include <boost/test/unit_test.hpp>
#include "events/EventMgr.h"

using namespace event;

void eventListener(Event& event)
{
    std::clog << "EVENT: " << event.eventName() << " {"
	      << event << " }"<< std::endl;
}


BOOST_AUTO_TEST_CASE(eventmanager_test)
{
    new EventMgr;

    EventMgr *em = nullptr;
    em = EventMgr::singletonPtr();

    BOOST_REQUIRE( em );

    BOOST_CHECK_EQUAL( em->listenersCount(), 0 );

    auto el = std::make_shared<EventListener>(eventListener);
    em->addListener(ACTOR_COLLIDED, el);

    BOOST_CHECK_EQUAL( em->listenersCount(), 1 );

    em->removeListener(ACTOR_COLLIDED, el);

    BOOST_CHECK_EQUAL( em->listenersCount(), 0 );

    delete EventMgr::singletonPtr();
}
