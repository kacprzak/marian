#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE EventManagerTest
#include <boost/test/unit_test.hpp>
#include "Logger.h"


BOOST_AUTO_TEST_CASE(logger_test)
{
    PLOG << "test";
}
