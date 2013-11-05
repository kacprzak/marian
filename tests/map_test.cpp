#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MapTest
#include <boost/test/unit_test.hpp>
#include "TmxMap.h"

BOOST_AUTO_TEST_CASE(map_test)
{
    tmx::Map map;
    map.loadFromFile("../assets/map1.tmx");

    BOOST_CHECK( map.width == 20 );
    BOOST_CHECK( map.height == 20 );
    BOOST_CHECK( map.tileWidth == 32 );
    BOOST_CHECK( map.tileHeight == 32 );

    BOOST_TEST_MESSAGE( "Layers size == " << map.layers.size() );
    BOOST_CHECK( map.layers.size() == 4 );

    BOOST_CHECK( map.objectGroups.size() == 1 );
    BOOST_CHECK( map.objectGroups[0].objects.size() == 1 );

    BOOST_CHECK( map.layers[0].data.size() > 1 );
}
