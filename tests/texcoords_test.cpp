/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TexCoordsTest
#include <boost/test/unit_test.hpp>

#include "graphics/TexCoords.h"
#include "Util.h"

using namespace gfx;

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
    TexCoords<4> tc = calculateTexCoords(td.texWidth, td.texheight, td.tileCoords);

    BOOST_CHECK_CLOSE(tc[0].s, 0.0829015544041f, 0.000001f);
    BOOST_CHECK_CLOSE(tc[0].t, 0.108474576271f, 0.000001f);
    BOOST_CHECK_CLOSE(tc[2].s, 0.165803108808f, 0.000001f);
    BOOST_CHECK_CLOSE(tc[2].t, 0.216949152542f, 0.000001f);
}


BOOST_AUTO_TEST_CASE(flipVerticallyTexCoords_test)
{
    TestData td;
    TexCoords<4> tc = calculateTexCoords(td.texWidth, td.texheight, td.tileCoords);

    TexCoords<4> flipped = flipVerticallyTexCoords(tc);

    BOOST_CHECK_NE(tc, flipped);

    BOOST_CHECK_EQUAL(tc[0], flipped[1]);
    BOOST_CHECK_EQUAL(tc[1], flipped[0]);

    BOOST_CHECK_EQUAL(tc[3], flipped[2]);
    BOOST_CHECK_EQUAL(tc[2], flipped[3]);
}


BOOST_AUTO_TEST_CASE(flipHorizontallyTexCoords_test)
{
    TestData td;
    TexCoords<4> tc = calculateTexCoords(td.texWidth, td.texheight, td.tileCoords);

    TexCoords<4> flipped = flipHorizontallyTexCoords(tc);

    BOOST_CHECK_NE(tc, flipped);

    BOOST_CHECK_EQUAL(tc[0], flipped[3]);
    BOOST_CHECK_EQUAL(tc[1], flipped[2]);

    BOOST_CHECK_EQUAL(tc[3], flipped[0]);
    BOOST_CHECK_EQUAL(tc[2], flipped[1]);
}
