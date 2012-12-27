#include "Map.h"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MapTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(map_test)
{
  Map map;
  map.loadFromFile("media/map1.tmx");

  BOOST_REQUIRE( map.width == 20 );
  BOOST_REQUIRE( map.height == 20 );
  BOOST_REQUIRE( map.tileWidth == 32 );
  BOOST_REQUIRE( map.tileHeight == 32 );
}
