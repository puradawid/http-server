#include <boost/test/unit_test.hpp>
#include <string>
#include <memory>
#include "../src/dispatcher.h"

BOOST_AUTO_TEST_SUITE(DispatcherTestSuite);

class EmptyResponseHandler : public Handler {
public:
    bool handled = false;
    Response handle(Request& req) {
        if (req.path() == "/" && req.method() == Method::GET)
            this->handled = true;
        return Response(Responses::RESP_200);
    }

    bool test(Request& req) {
        return false;
    }
};

BOOST_AUTO_TEST_CASE(DispatcherShouldReturnEmptyResponse)
{
    Dispatcher d;
    TestPortListener tpl(&d, "GET / HTTP/1.1\r\n\r\n");
    std::shared_ptr<EmptyResponseHandler> p = std::make_shared<EmptyResponseHandler>();

    d.add(p);

    std::string response = tpl.send();

	BOOST_CHECK(p->handled);
    BOOST_TEST(response == "HTTP/1.1 200 OK\r\n\r\n", "The response is '" + response + "' instead of 200 OK");
};

class ExpectedHeadersInRequest : public Handler {
public:
    bool handled = false;

    Response handle(Request& req) {
        if (req.path() == "/" && req.method() == Method::GET && req.headers().find("Host").value() == "test.com")
            this->handled = true;
        return Response(Responses::RESP_200);
    }

    bool test(Request& req) {
        return true;
    }
};

BOOST_AUTO_TEST_CASE(DispatcherShouldHandleHeaders)
{
    Dispatcher d;
    TestPortListener tpl(&d, "GET / HTTP/1.1\r\nHost: test.com\r\n\r\n");
    std::shared_ptr<ExpectedHeadersInRequest> p = std::make_shared<ExpectedHeadersInRequest>();
    std::shared_ptr<EmptyResponseHandler> additional = std::make_shared<EmptyResponseHandler>();

    d.add(p);
    d.add(additional);

    std::string response = tpl.send();

	BOOST_CHECK(p->handled);
    BOOST_CHECK(!additional->handled);
};


BOOST_AUTO_TEST_SUITE_END();