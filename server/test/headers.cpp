#include <boost/test/unit_test.hpp>
#include <string>
#include "../src/connection.h"

#include "../src/parser.h"
#include "../src/headers.h"

BOOST_AUTO_TEST_SUITE(HeadersTestSuite);

BOOST_AUTO_TEST_CASE(HeadersShouldReturnEmptyValue)
{
    Headers h;
    BOOST_CHECK(h.find("random").name() == "");
};

BOOST_AUTO_TEST_CASE(HeadersShouldReturnAValueIfAdded)
{
    Headers h;
    
    h.addHeader(Header("Random", "value"));

    BOOST_CHECK(h.find("random").name() == "Random");
    BOOST_CHECK(h.find("random").value() == "value");
};

BOOST_AUTO_TEST_SUITE_END();