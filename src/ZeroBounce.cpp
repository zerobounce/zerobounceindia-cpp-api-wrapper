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

void ZeroBounce::sendFile(
    std::string filePath,
    int emailAddressColumnIndex,
    SendFileOptions options,
    OnSuccessCallback<ZBSendFileResponse> successCallback,
    OnErrorCallback errorCallback
) {
    sendFileInternal(false, filePath, emailAddressColumnIndex, options, successCallback, errorCallback);
}

void ZeroBounce::fileStatus(
    std::string fileId,
    OnSuccessCallback<ZBFileStatusResponse> successCallback,
    OnErrorCallback errorCallback
) {
    fileStatusInternal(false, fileId, successCallback, errorCallback);
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

void ZeroBounce::sendFileInternal(
    bool scoring,
    std::string filePath,
    int emailAddressColumnIndex,
    SendFileOptions options,
    OnSuccessCallback<ZBSendFileResponse> successCallback,
    OnErrorCallback errorCallback
) {
    if (invalidApiKey(errorCallback)) return;

    std::string urlPath = (scoring ? bulkApiScoringBaseUrl : bulkApiBaseUrl) + "/sendFile";

    cpr::Multipart multipart{
        {"api_key", apiKey},
        {"file", cpr::File{filePath}},
        {"email_address_column", emailAddressColumnIndex}
    };

    if (!scoring) {
        if (options.firstNameColumn > 0) {
            multipart.parts.emplace_back(cpr::Part{"first_name_column", options.firstNameColumn});
        }
        if (options.lastNameColumn > 0) {
            multipart.parts.emplace_back(cpr::Part{"last_name_column", options.lastNameColumn});
        }
        if (options.genderColumn > 0) {
            multipart.parts.emplace_back(cpr::Part{"gender_column", options.genderColumn});
        }
        if (options.ipAddressColumn > 0) {
            multipart.parts.emplace_back(cpr::Part{"ip_address_column", options.ipAddressColumn});
        }
    }

    if (!options.returnUrl.empty()) {
            multipart.parts.emplace_back(cpr::Part{"return_url", options.returnUrl});
    }
    
    multipart.parts.emplace_back(cpr::Part{"has_header_row", options.hasHeaderRow});
    multipart.parts.emplace_back(cpr::Part{"remove_duplicate", options.removeDuplicate});

    cpr::Response r = cpr::Post(
        cpr::Url{urlPath},
        cpr::Header{{"Content-Type", "multipart/form-data"}},
        multipart
    );

    std::string rsp = r.text;

    if (r.status_code > 299) {
        if (errorCallback) {
            ZBErrorResponse errorResponse = ZBErrorResponse::parseError(rsp);
            errorCallback(errorResponse);
        }
    } else {
        if (successCallback) {
            ZBSendFileResponse response = ZBSendFileResponse::from_json(json::parse(rsp));
            successCallback(response);
        }
    }
}

void ZeroBounce::fileStatusInternal(
    bool scoring,
    std::string fileId,
    OnSuccessCallback<ZBFileStatusResponse> successCallback,
    OnErrorCallback errorCallback
) {
    if (invalidApiKey(errorCallback)) return;

    sendRequest(
        (scoring ? bulkApiScoringBaseUrl : bulkApiBaseUrl) + "/filestatus?api_key=" + apiKey
            + "&file_id=" + fileId,
        successCallback,
        errorCallback
    );
}
