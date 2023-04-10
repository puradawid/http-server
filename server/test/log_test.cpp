#include <boost/test/unit_test.hpp>
#include <string>
#include "../src/log.h"

BOOST_AUTO_TEST_SUITE(LogTestSuite);

BOOST_AUTO_TEST_CASE(ConfigEntryShouldParseTheRightParameters)
{
    Logging::ConfigEntry ce("Tcp.*:file:warn");

	BOOST_CHECK(ce.regex() == "Tcp.*");
    BOOST_TEST(ce.loggerFactoryName() == "file");
    BOOST_CHECK(ce.level() == "warn");
};

BOOST_AUTO_TEST_SUITE_END();
