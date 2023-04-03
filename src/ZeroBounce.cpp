#include <ctime>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "ZeroBounce/ZeroBounce.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

ZeroBounce::ZeroBounce() {
    curl_global_init(CURL_GLOBAL_ALL);
}

ZeroBounce::~ZeroBounce() {
    curl_global_cleanup();
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

    try {
        std::string urlPath = bulkApiBaseUrl + "/validatebatch";

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

        CURL* curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to initialize libcurl");
        }

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, urlPath.c_str());

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, payload.dump().c_str());

        std::string responseData;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ZeroBounce::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        CURLcode res = curl_easy_perform(curl);

        long httpCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

        if (httpCode > 299) {
            if (errorCallback) {
                ZBErrorResponse errorResponse = ZBErrorResponse::parseError(responseData);
                errorCallback(errorResponse);
            }
        } else {
            if (successCallback) {
                ZBValidateBatchResponse response = ZBValidateBatchResponse::from_json(json::parse(responseData));
                successCallback(response);
            }
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    } catch (std::exception e) {
        ZBErrorResponse errorResponse = ZBErrorResponse::parseError(e.what());
        errorCallback(errorResponse);
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

void ZeroBounce::getFile(
    std::string fileId,
    std::string localDownloadPath,
    OnSuccessCallback<ZBGetFileResponse> successCallback,
    OnErrorCallback errorCallback
) {
    getFileInternal(false, fileId, localDownloadPath, successCallback, errorCallback);
}

void ZeroBounce::deleteFile(
    std::string fileId,
    OnSuccessCallback<ZBDeleteFileResponse> successCallback,
    OnErrorCallback errorCallback
) {
    deleteFileInternal(false, fileId, successCallback, errorCallback);
}

void ZeroBounce::scoringSendFile(
    std::string filePath,
    int emailAddressColumnIndex,
    SendFileOptions options,
    OnSuccessCallback<ZBSendFileResponse> successCallback,
    OnErrorCallback errorCallback
) {
    sendFileInternal(true, filePath, emailAddressColumnIndex, options, successCallback, errorCallback);
}

void ZeroBounce::scoringFileStatus(
    std::string fileId,
    OnSuccessCallback<ZBFileStatusResponse> successCallback,
    OnErrorCallback errorCallback
) {
    fileStatusInternal(true, fileId, successCallback, errorCallback);
}

void ZeroBounce::scoringGetFile(
    std::string fileId,
    std::string localDownloadPath,
    OnSuccessCallback<ZBGetFileResponse> successCallback,
    OnErrorCallback errorCallback
) {
    getFileInternal(true, fileId, localDownloadPath, successCallback, errorCallback);
}

void ZeroBounce::scoringDeleteFile(
    std::string fileId,
    OnSuccessCallback<ZBDeleteFileResponse> successCallback,
    OnErrorCallback errorCallback
) {
    deleteFileInternal(true, fileId, successCallback, errorCallback);
}

void ZeroBounce::getActivityData(
    std::string email,
    OnSuccessCallback<ZBActivityDataResponse> successCallback,
    OnErrorCallback errorCallback
) {
    if (invalidApiKey(errorCallback)) return;

    sendRequest(
        apiBaseUrl + "/activity?api_key=" + apiKey + "&email=" + email,
        successCallback,
        errorCallback
    );
}

size_t ZeroBounce::writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

template <typename T>
void ZeroBounce::sendRequest(
    std::string urlPath,
    OnSuccessCallback<T> successCallback,
    OnErrorCallback errorCallback
) {
    try {
        CURL* curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to initialize libcurl");
        }

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, urlPath.c_str());

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Accept: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        std::string responseData;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ZeroBounce::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        CURLcode res = curl_easy_perform(curl);

        long httpCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

        if (httpCode > 299) {
            if (errorCallback) {
                ZBErrorResponse errorResponse = ZBErrorResponse::parseError(responseData);
                errorCallback(errorResponse);
            }
        } else {
            if (successCallback) {
                T response = T::from_json(json::parse(responseData));
                successCallback(response);
            }
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    } catch (std::exception e) {
        ZBErrorResponse errorResponse = ZBErrorResponse::parseError(e.what());
        errorCallback(errorResponse);
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

    try {
        std::string urlPath = (scoring ? bulkApiScoringBaseUrl : bulkApiBaseUrl) + "/sendFile";

        CURL* curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to initialize libcurl");
        }

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, urlPath.c_str());

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: multipart/form-data");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_mime *multipart = curl_mime_init(curl);

        curl_mimepart *part = curl_mime_addpart(multipart);
        curl_mime_name(part, "api_key");
        curl_mime_data(part, apiKey.c_str(), CURL_ZERO_TERMINATED);

        part = curl_mime_addpart(multipart);
        curl_mime_name(part, "file");
        curl_mime_filedata(part, filePath.c_str());

        part = curl_mime_addpart(multipart);
        curl_mime_name(part, "email_address_column");
        curl_mime_data(part, std::to_string(emailAddressColumnIndex).c_str(), CURL_ZERO_TERMINATED);

        if (!scoring) {
            if (options.firstNameColumn > 0) {
                part = curl_mime_addpart(multipart);
                curl_mime_name(part, "first_name_column");
                curl_mime_data(part, std::to_string(options.firstNameColumn).c_str(), CURL_ZERO_TERMINATED);
            }
            if (options.lastNameColumn > 0) {
                part = curl_mime_addpart(multipart);
                curl_mime_name(part, "last_name_column");
                curl_mime_data(part, std::to_string(options.lastNameColumn).c_str(), CURL_ZERO_TERMINATED);
            }
            if (options.genderColumn > 0) {
                part = curl_mime_addpart(multipart);
                curl_mime_name(part, "gender_column");
                curl_mime_data(part, std::to_string(options.genderColumn).c_str(), CURL_ZERO_TERMINATED);
            }
            if (options.ipAddressColumn > 0) {
                part = curl_mime_addpart(multipart);
                curl_mime_name(part, "ip_address_column");
                curl_mime_data(part, std::to_string(options.ipAddressColumn).c_str(), CURL_ZERO_TERMINATED);
            }
        }

        if (!options.returnUrl.empty()) {
            part = curl_mime_addpart(multipart);
            curl_mime_name(part, "return_url");
            curl_mime_data(part, options.returnUrl.c_str(), CURL_ZERO_TERMINATED);
        }

        part = curl_mime_addpart(multipart);
        curl_mime_name(part, "has_header_row");
        curl_mime_data(part, options.hasHeaderRow ? "true" : "false", CURL_ZERO_TERMINATED);

        part = curl_mime_addpart(multipart);
        curl_mime_name(part, "remove_duplicate");
        curl_mime_data(part, options.removeDuplicate ? "true" : "false", CURL_ZERO_TERMINATED);

        curl_easy_setopt(curl, CURLOPT_MIMEPOST, multipart);

        std::string responseData;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ZeroBounce::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        CURLcode res = curl_easy_perform(curl);

        long httpCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

        if (httpCode > 299) {
            if (errorCallback) {
                ZBErrorResponse errorResponse = ZBErrorResponse::parseError(responseData);
                errorCallback(errorResponse);
            }
        } else {
            if (successCallback) {
                ZBSendFileResponse response = ZBSendFileResponse::from_json(json::parse(responseData));
                successCallback(response);
            }
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        curl_mime_free(multipart);
    } catch (std::exception e) {
        ZBErrorResponse errorResponse = ZBErrorResponse::parseError(e.what());
        errorCallback(errorResponse);
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

void ZeroBounce::getFileInternal(
    bool scoring,
    std::string fileId,
    std::string localDownloadPath,
    OnSuccessCallback<ZBGetFileResponse> successCallback,
    OnErrorCallback errorCallback
) {
    if (invalidApiKey(errorCallback)) return;

    try {
        std::string urlPath = (scoring ? bulkApiScoringBaseUrl : bulkApiBaseUrl)
            + "/getFile?api_key=" + apiKey + "&file_id=" + fileId;
        
        CURL* curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to initialize libcurl");
        }

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, urlPath.c_str());

        std::string responseData;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ZeroBounce::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        CURLcode res = curl_easy_perform(curl);

        long httpCode = 0;
        struct curl_header *contentTypeHeader;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        curl_easy_header(curl, "Content-Type", 0, CURLH_HEADER, -1, &contentTypeHeader);

        if (httpCode > 299) {
            if (errorCallback) {
                ZBErrorResponse errorResponse = ZBErrorResponse::parseError(responseData);
                errorCallback(errorResponse);
            }
        } else {
            if (successCallback) {
                if (strcmp(contentTypeHeader->value, "application/json") != 0) {
                    fs::path filePath(localDownloadPath);

                    if (fs::is_directory(filePath)) {
                        ZBErrorResponse errorResponse = ZBErrorResponse::parseError("Invalid file path");
                        errorCallback(errorResponse);
                        return;
                    }

                    fs::create_directories(filePath.parent_path());

                    std::ofstream fileStream(filePath, std::ofstream::out | std::ofstream::binary);

                    fileStream.write(responseData.c_str(), responseData.size());
                    fileStream.close();

                    ZBGetFileResponse response;
                    response.success = true;
                    response.localFilePath = localDownloadPath;
                    successCallback(response);
                } else {
                    ZBGetFileResponse response = ZBGetFileResponse::from_json(json::parse(responseData));
                    successCallback(response);
                }
            }
        }

        curl_easy_cleanup(curl);
    } catch (std::exception e) {
        ZBErrorResponse errorResponse = ZBErrorResponse::parseError(e.what());
        errorCallback(errorResponse);
    }
}

void ZeroBounce::deleteFileInternal(
    bool scoring,
    std::string fileId,
    OnSuccessCallback<ZBDeleteFileResponse> successCallback,
    OnErrorCallback errorCallback
) {
    if (invalidApiKey(errorCallback)) return;

    sendRequest(
        (scoring ? bulkApiScoringBaseUrl : bulkApiBaseUrl) + "/deletefile?api_key=" + apiKey
            + "&file_id=" + fileId,
        successCallback,
        errorCallback
    );
}
