#include <ctime>
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
        apiBaseUrl + "/getcredits?api_key=" + apiKey,
        successCallback,
        errorCallback
    );

}

void ZeroBounce::getApiUsage(
    std::tm startDate,
    std::tm endDate,
    OnSuccessCallback<ZBGetApiUsageResponse> successCallback,
    OnErrorCallback errorCallback
) {
    if (invalidApiKey(errorCallback)) return;

    std::string dateFormat = "%Y-%m-%d";
    std::ostringstream url;

    url << apiBaseUrl << "/getapiusage?api_key=" << apiKey
        << "&start_date=" << std::put_time(&startDate, dateFormat.c_str())
        << "&end_date=" << std::put_time(&endDate, dateFormat.c_str());

    sendRequest(
        url.str(),
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
