#include <boost/test/unit_test.hpp>
#include <string>
#include <memory>
#include "../src/dispatcher.h"

BOOST_AUTO_TEST_SUITE(ResponseTestSuite);

BOOST_AUTO_TEST_CASE(ResponseShouldSerializeToJson)
{
    Response r(Responses::RESP_200);
    r.content("{ test: value }", "application/json");
    std::string result = r.serialize();

    BOOST_TEST(result == "HTTP/1.1 200 OK\r\nContent-Length: 15\r\nContent-Type: application/json\r\n\r\n{ test: value }");
}

BOOST_AUTO_TEST_SUITE_END();