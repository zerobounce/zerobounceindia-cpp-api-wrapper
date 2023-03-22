#include <iostream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "ZeroBounce/ZeroBounce.h"

using json = nlohmann::json;

ZeroBounce::ZeroBounce() {
}

ZeroBounce* ZeroBounce::instance = nullptr;

ZeroBounce* ZeroBounce::getInstance() {
    if (ZeroBounce::instance == nullptr) {
        ZeroBounce::instance = new ZeroBounce();
    }
    return ZeroBounce::instance;
}

void ZeroBounce::initialize(std::string apiKey) {
    this->apiKey = apiKey;
}

bool ZeroBounce::invalidApiKey(OnErrorCallback errorCallback) {
    if (apiKey.empty()) {
            ZBErrorResponse errorResponse = ZBErrorResponse::parseError(
                "ZeroBounce is not initialized. Please call ZeroBounce::getInstance()->initialize(apiKey) first"
            );
            errorCallback(errorResponse);
            return true;
        }
    return false;
}

void ZeroBounce::getCredits(
    OnSuccessCallback<ZBCreditsResponse> successCallback,
    OnErrorCallback errorCallback
) {
    if (invalidApiKey(errorCallback)) return;

    sendRequest(
        apiBaseUrl + "/getcredits",
        successCallback,
        errorCallback
    );

}

template <typename T>
void ZeroBounce::sendRequest(
    std::string urlPath,
    OnSuccessCallback<T> successCallback,
    OnErrorCallback errorCallback
) {
    cpr::Response r = cpr::Get(
        cpr::Url{urlPath},
        cpr::Parameters{{"api_key", apiKey}},
        cpr::Header{{"Accept", "application/json"}}
    );
    
    std::string rsp = r.text;

    if (r.status_code > 299) {
        if (errorCallback) {
            ZBErrorResponse errorResponse = ZBErrorResponse::parseError(rsp);
            errorCallback(errorResponse);
        }
    } else {
        if (successCallback) {
            T response = T::from_json(json::parse(rsp));
            successCallback(response);
        }
    }
}
