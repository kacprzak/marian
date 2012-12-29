#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MathTest
#include <boost/test/unit_test.hpp>
#include "Math.h"

BOOST_AUTO_TEST_CASE(math_test)
{
  Rect<int> rectA(10, 10, 20, 20);
  Rect<int> rectB(20, 20, 30, 30);
  Rect<int> rectC(70, 70, 20, 20);

  BOOST_REQUIRE( rectA.intersects(rectB) == true );
  BOOST_REQUIRE( rectA.intersects(rectC) == false );
}
