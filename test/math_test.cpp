#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MathTest
#include <boost/test/unit_test.hpp>
#include "Math.h"

BOOST_AUTO_TEST_CASE(intersects_test)
{
  Rect<int> rectA(10, 10, 20, 20);
  Rect<int> rectB(20, 20, 30, 30);
  Rect<int> rectC(70, 70, 20, 20);

  BOOST_REQUIRE( rectA.intersects(rectB) == true );
  BOOST_REQUIRE( rectA.intersects(rectC) == false );
}

BOOST_AUTO_TEST_CASE(penetrationVector_test)
{
  {
    Rect<int> rectA(1, 1, 2, 2);
    Rect<int> rectB(2, 2, 4, 4);
    Rect<int> rectC(7, 7, 2, 2);

    Vector2<int> ab = rectA.penetrationVector(rectB);
    Vector2<int> ac = rectA.penetrationVector(rectC);
    
    BOOST_REQUIRE( !ab.isNull() );
    BOOST_REQUIRE( ac.isNull() );
    
    BOOST_REQUIRE_EQUAL( ab.x, -1 );
    BOOST_REQUIRE_EQUAL( ab.y, -1 );
  }

  {
    Rect<int> rectD(-1, -1, 2, 2);
    Rect<int> rectE(-2, -2, 4, 4);
    Rect<int> rectF(-7, -7, 2, 2);

    Vector2<int> de = rectD.penetrationVector(rectE);
    Vector2<int> df = rectD.penetrationVector(rectF);
    
    BOOST_REQUIRE( !de.isNull() );
    BOOST_REQUIRE( df.isNull() );
    
    BOOST_REQUIRE_EQUAL( de.x, 3 );
    BOOST_REQUIRE_EQUAL( de.y, 3 );
  }
}
