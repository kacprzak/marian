/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TexCoordsTest
#include <boost/test/unit_test.hpp>

#include "graphics/TexCoords.h"
#include "Util.h"

struct TestData
{
    TestData()
    {
        texWidth = 386;
        texheight = 295;

        tileCoords.left = 32;
        tileCoords.bottom = 32;
        tileCoords.right = 64;
        tileCoords.top = 64;
    }

    int texWidth;
    int texheight;

    Rect<int> tileCoords;
};

BOOST_AUTO_TEST_CASE(calculateTexCoords_test)
{
    TestData td;
    TexCoords<4> tc = calculateTexCoords(td.texWidth, td.texheight,
                                         td.tileCoords.left, td.tileCoords.bottom,
                                         td.tileCoords.right, td.tileCoords.top);

    BOOST_CHECK_CLOSE(tc.coords[0].s, 0.0829015544041f, 0.0001f);
    BOOST_CHECK_CLOSE(tc.coords[0].t, 0.108474576271f, 0.0001f);
    BOOST_CHECK_CLOSE(tc.coords[2].s, 0.165803108808f, 0.0001f);
    BOOST_CHECK_CLOSE(tc.coords[2].t, 0.216949152542f, 0.0001f);
}

BOOST_AUTO_TEST_CASE(flipVerticallyTexCoords_test)
{
    //BOOST_CHECK_EQUAL(1, 2);
}
