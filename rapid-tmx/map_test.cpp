/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MapTest
#include <boost/test/unit_test.hpp>
#include "TmxMap.h"

BOOST_AUTO_TEST_CASE(map_test)
{
    tmx::Map map;
    map.loadFromFile("../map1.tmx");

    BOOST_CHECK_EQUAL( map.width, 20 );
    BOOST_CHECK_EQUAL( map.height, 20 );
    BOOST_CHECK_EQUAL( map.tileWidth, 32 );
    BOOST_CHECK_EQUAL( map.tileHeight, 32 );

    BOOST_TEST_MESSAGE( "Layers size == " << map.layers.size() );
    BOOST_CHECK_EQUAL( map.layers.size(), 4 );

    BOOST_CHECK_EQUAL( map.objectGroups.size(), 1 );
    BOOST_CHECK_EQUAL( map.objectGroups[0].objects.size(), 1 );

    BOOST_CHECK_GT( map.layers[0].data.size(), 1 );
}
