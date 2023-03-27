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

void ZeroBounce::validate(
    std::string email,
    std::string ipAddress,
    OnSuccessCallback<ZBValidateResponse> successCallback,
    OnErrorCallback errorCallback
) {
    if (invalidApiKey(errorCallback)) return;

    sendRequest(
        apiBaseUrl + "/validate?api_key=" + apiKey +
            "&email=" + email +
            "&ip_address=" + (ipAddress.empty() ? "" : ipAddress),
        successCallback,
        errorCallback
    );
}

void ZeroBounce::validateBatch(
    std::vector<ZBEmailToValidate> emailBatch,
    OnSuccessCallback<ZBValidateBatchResponse> successCallback,
    OnErrorCallback errorCallback
) {
    if (invalidApiKey(errorCallback)) return;

    json payload;
    payload["api_key"] = apiKey;

    for (auto& email : emailBatch) {
        json emailObj;
        emailObj["email_address"] = email.emailAddress;

        if (email.ipAddress.empty()) {
            emailObj["ip_address"] = json::value_t::null;
        } else {
            emailObj["ip_address"] = email.ipAddress;
        }
        
        payload["email_batch"].push_back(emailObj);
    }

    cpr::Response r = cpr::Post(
        cpr::Url{bulkApiBaseUrl + "/validatebatch"},
        cpr::Header{
            {"Accept", "application/json"},
            {"Content-Type", "application/json"}
        },
        cpr::Body{payload.dump()}
    );
    
    std::string rsp = r.text;

    if (r.status_code > 299) {
        if (errorCallback) {
            ZBErrorResponse errorResponse = ZBErrorResponse::parseError(rsp);
            errorCallback(errorResponse);
        }
    } else {
        if (successCallback) {
            ZBValidateBatchResponse response = ZBValidateBatchResponse::from_json(json::parse(rsp));
            successCallback(response);
        }
    }
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
