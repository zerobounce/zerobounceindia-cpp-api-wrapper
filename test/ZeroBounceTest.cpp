#include <string>

#include <cpr/cpr.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ZeroBounce/ZeroBounce.h"

class ZeroBounceTest : public ZeroBounce {
    private:
        static ZeroBounceTest* instance;
    public:
        void setRequestHandler(RequestHandler* requestHandler) {
            this->requestHandler = requestHandler;
        }

        static ZeroBounceTest* getInstance() {
            if (ZeroBounceTest::instance == nullptr) {
                ZeroBounceTest::instance = new ZeroBounceTest();
            }
            return ZeroBounceTest::instance;
        }
};

ZeroBounceTest* ZeroBounceTest::instance = nullptr;

class MockRequestHandler : public RequestHandler {
    private:
        cpr::Response response;
    public:
        void setResponse(cpr::Response response) {
            this->response = response;
        }

        template <typename... Ts>
        cpr::Response Get(Ts&&... ts) {
            return response;
        };

        template <typename... Ts>
        cpr::Response Post(Ts&&... ts) {
            return response;
        };
};

class Tests : public ::testing::Test {
    protected:
        const std::string API_KEY = "api-key";
        MockRequestHandler* mockRequestHandler;

        void SetUp() override {
            mockRequestHandler = new MockRequestHandler();
            ZeroBounceTest::getInstance()->initialize(API_KEY);
        }

        void TearDown() override {
            delete mockRequestHandler;
        }
};

TEST_F(Tests, testGetCreditsValid) {
    std::string responseJson = "{\"Credits\":\"2375323\"}";

    cpr::Response reqResponse;
    reqResponse.text = responseJson;
    reqResponse.status_code = 200;

    ZBCreditsResponse expectedResponse = ZBCreditsResponse::from_json(json::parse(responseJson));
    
    mockRequestHandler->setResponse(reqResponse);
    ZeroBounceTest::getInstance()->setRequestHandler(mockRequestHandler);

    ZeroBounceTest::getInstance()->getCredits(
        [](ZBCreditsResponse response) {
            std::cout << response.toString() << std::endl;
        },
        [](ZBErrorResponse errorResponse) {
            std::cout << errorResponse.toString() << std::endl;
        }
    );
}
