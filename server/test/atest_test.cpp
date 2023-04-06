#include <boost/test/unit_test.hpp>
#include <string>
#include "../src/connection.h"

#include "../src/parser.h"

BOOST_AUTO_TEST_SUITE(Network);

BOOST_AUTO_TEST_CASE(ParserCase)
{
    MessageChunk m("this is message");

	BOOST_CHECK(m.message() == "this is message");
};

BOOST_AUTO_TEST_CASE(shouldCreateMessageWithCharPointerNetwork)
{
    char *s = "test 123";
    MessageChunk m1(s, 4);

    BOOST_CHECK(m1.message() == "test");
};

BOOST_AUTO_TEST_SUITE_END();
