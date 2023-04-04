#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE HttpServer
#include <boost/test/unit_test.hpp>
#include <string>
#include "../src/connection.h"

#include "../src/parser.h"

std::list<MessageChunk> createChunks(std::string request) {
    std::list<MessageChunk> messages;

    for (unsigned i = 0; i < request.length(); i += 255) {
        std::string message = request.substr(i, i + 255);
        MessageChunk m(message);
        messages.push_back(message);
    }

    return messages;
}

BOOST_AUTO_TEST_SUITE(ParserTestingSuite);

BOOST_AUTO_TEST_CASE(ParserCase)
{
    MessageChunk m("this is message");

	BOOST_CHECK(m.message() == "this is message");
};

BOOST_AUTO_TEST_CASE(shouldCreateMessageWithCharPointer)
{
    char *s = "test 123";
    MessageChunk m1(s, 4);

    BOOST_CHECK(m1.message() == "test");
};

BOOST_AUTO_TEST_CASE(shouldHandleRealHttpRequest)
{
    std::string request = "GET / HTTP/1.1\nHost: localhost:1231\nUser-Agent: curl/7.81.0\nAccept: */*\nreport-to: {\"group\":\"gwsaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaacbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa-team\",\"max_age\":2592000,\"endpoints\":[{\"url\":\"https://csp.withgoogle.com/csp/report-to/gws-team\"}]}\n\n";
    std::list<MessageChunk> messages = createChunks(request);

    HttpParser parser;

    for (MessageChunk s : messages) {
        Continue c = parser.digest(s);
        BOOST_CHECK(c.continueProcessing);
    }

    Request r = parser.build();
    BOOST_CHECK(r.method() == Method::GET);
}

BOOST_AUTO_TEST_CASE(shouldHandleEmptyHttpPostRequest)
{
    std::string request = "POST / HTTP/1.1\n\n";
    std::list<MessageChunk> messages = createChunks(request);

    HttpParser parser;

    parser.digest(request);

    Request r = parser.build();
    BOOST_CHECK(r.method() == Method::POST);
}

BOOST_AUTO_TEST_CASE(shouldHandlePathPostRequest)
{
    std::string request = "POST /this-is-my/path?q=123123 HTTP/1.1\n\n";
    HttpParser parser;

    parser.digest(request);

    Request r = parser.build();
    BOOST_CHECK(r.path() == "/this-is-my/path?q=123123");
}


BOOST_AUTO_TEST_CASE(shouldStopAfterCompleteRequest)
{
    std::string request = "POST /this-is-my/path?q=123123 HTTP/1.1\n\n";
    HttpParser parser;

    Continue c = parser.digest(request);

    BOOST_CHECK(!c.continueProcessing);
}

BOOST_AUTO_TEST_CASE(shouldParseTheOnlyHeader)
{
    std::string request = "POST /this-is-my/path?q=123123 HTTP/1.1\nHost: test.com\n\n";

    HttpParser parser;

    parser.digest(PreambleChunk(request, Method::GET, "/"));

    Request r = parser.build();

    BOOST_TEST(r.header("Host") == "test.com", r.header("Host") + " is not test.com");
}

BOOST_AUTO_TEST_CASE(HeadersParsing)
{
    std::string request = "Host: test.com\n\n";

    HeadersParser parser;

    parser.digest(PreambleChunk(request, Method::GET, "/"));

    Request r = parser.build();

    BOOST_TEST(r.header("Host") == "test.com", r.header("Host") + " is not test.com");
}

BOOST_AUTO_TEST_SUITE_END();
