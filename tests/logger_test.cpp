/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE LoggerTest
#include <boost/test/unit_test.hpp>
#include "Logger.h"


BOOST_AUTO_TEST_CASE(logger_test)
{
    PLOG << "test";
}
