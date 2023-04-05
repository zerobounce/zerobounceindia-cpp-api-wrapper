#include <string>

#include <cpr/cpr.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ZeroBounce/ZeroBounce.h"

class MockRequestHandler : public BaseRequestHandler {
    private:
        cpr::Response response;

    protected:
        cpr::Response doGet(const cpr::Url& url) {
            return response;
        }
        cpr::Response doGet(const cpr::Url& url, const cpr::Header& header) {
            return response;
        }
        cpr::Response doPost(const cpr::Url& url, const cpr::Header& header, const cpr::Body& body) {
            return response;
        }
        cpr::Response doPost(const cpr::Url& url, const cpr::Header& header, const cpr::Multipart& multipart) {
            return response;
        }

    public:
        void setResponse(cpr::Response response) {
            this->response = response;
        }
};

class ZeroBounceTest : public ZeroBounce {
    private:
        static ZeroBounceTest* instance;

    public:
        void setRequestHandler(BaseRequestHandler* requestHandler) {
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

cpr::Response mockResponse(std::string content, long statusCode) {
    cpr::Response reqResponse;

    reqResponse.text = content;
    reqResponse.status_code = statusCode;

    return reqResponse;
}

class Tests : public ::testing::Test {
    protected:
        const std::string API_KEY = "api-key";
        MockRequestHandler* mockRequestHandler;

        void SetUp() override {
            mockRequestHandler = new MockRequestHandler();
            ZeroBounceTest::getInstance()->initialize(API_KEY);
            ZeroBounceTest::getInstance()->setRequestHandler(mockRequestHandler);
        }

        void TearDown() override {
            delete mockRequestHandler;
        }
};

TEST_F(Tests, testGetCreditsInvalid) {
    std::string responseJson = "{\"Credits\":\"-1\"}";

    cpr::Response reqResponse = mockResponse(responseJson, 400);
    mockRequestHandler->setResponse(reqResponse);

    ZBErrorResponse expectedResponse = ZBErrorResponse::parseError(responseJson);

    ZeroBounceTest::getInstance()->getCredits(
        [&](ZBCreditsResponse response) {
            FAIL() << response.toString();
        },
        [&](ZBErrorResponse errorResponse) {
            ASSERT_EQ(errorResponse, expectedResponse);
        }
    );
}

TEST_F(Tests, testGetCreditsValid) {
    std::string responseJson = "{\"Credits\":\"100\"}";

    cpr::Response reqResponse = mockResponse(responseJson, 200);
    mockRequestHandler->setResponse(reqResponse);

    ZBCreditsResponse expectedResponse = ZBCreditsResponse::from_json(json::parse(responseJson));
    
    ZeroBounceTest::getInstance()->getCredits(
        [&](ZBCreditsResponse response) {
            ASSERT_EQ(response, expectedResponse);
        },
        [&](ZBErrorResponse errorResponse) {
            FAIL() << errorResponse.toString();
        }
    );
}
