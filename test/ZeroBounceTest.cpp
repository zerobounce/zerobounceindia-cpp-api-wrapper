#include <iostream>
#include <string>

#include "ZeroBounce/ZeroBounce.h"

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(ZeroBounceTest) {
    ZeroBounce* zb;

    void setup() {
        zb = ZeroBounce::getInstance();
        zb->initialize("api-key");
    }

    void teardown() {
        delete zb;
    }
};

TEST(ZeroBounceTest, testGetCreditsValid) {
    std::string responseJson = "{\"Credits\":\"2375323\"}";

    ZBCreditsResponse expectedResponse = ZBCreditsResponse::from_json(json::parse(responseJson));
    
    zb->getCredits(
        [](ZBCreditsResponse response) {
            std::cout << response.toString() << std::endl;
        },
        [](ZBErrorResponse errorResponse) {
            std::cout << errorResponse.toString() << std::endl;
        }
    );
}

int main(int argc, char** argv) {
    return CommandLineTestRunner::RunAllTests(argc, argv);
}

