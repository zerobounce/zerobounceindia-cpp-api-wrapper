#ifndef ZEROBOUNCE_H
#define ZEROBOUNCE_H

#include <functional>
#include <string>
#include <vector>

#include "ZeroBounce/ZBErrorResponse.h"
#include "ZeroBounce/ZBCreditsResponse.h"
#include "ZeroBounce/ZBGetApiUsageResponse.h"
#include "ZeroBounce/ZBValidateResponse.h"
#include "ZeroBounce/ZBValidateBatchResponse.h"
#include "ZeroBounce/ZBSendFileResponse.h"
#include "ZeroBounce/ZBFileStatusResponse.h"
#include "ZeroBounce/ZBGetFileResponse.h"
#include "ZeroBounce/ZBDeleteFileResponse.h"
#include "ZeroBounce/ZBActivityDataResponse.h"

#include <cpr/cpr.h>

template<typename T>
using OnSuccessCallback = std::function<void(T response)>;

using OnErrorCallback = std::function<void(ZBErrorResponse errorResponse)>;

struct SendFileOptions {
    std::string returnUrl = "";
    int firstNameColumn = 0;
    int lastNameColumn = 0;
    int genderColumn = 0;
    int ipAddressColumn = 0;
    bool hasHeaderRow = true;
    bool removeDuplicate = true;
};

class RequestHandler {
    public:
        template <typename... Ts>
        cpr::Response Get(Ts&&... ts) {
            return cpr::Get(std::forward<Ts>(ts)...);
        };

        template <typename... Ts>
        cpr::Response Post(Ts&&... ts) {
            return cpr::Post(std::forward<Ts>(ts)...);
        };
};

class ZeroBounce {
    protected:
        RequestHandler* requestHandler;
    private:
        static ZeroBounce* instance;
        std::string apiKey;
        const std::string apiBaseUrl = "https://api.zerobounce.net/v2";
        const std::string bulkApiBaseUrl = "https://bulkapi.zerobounce.net/v2";
        const std::string bulkApiScoringBaseUrl = "https://bulkapi.zerobounce.net/v2/scoring";

        bool invalidApiKey(OnErrorCallback errorCallback);

        template <typename T>
        void sendRequest(
            std::string urlPath,
            OnSuccessCallback<T> successCallback,
            OnErrorCallback errorCallback
        );

        void sendFileInternal(
            bool scoring,
            std::string filePath,
            int emailAddressColumnIndex,
            SendFileOptions options,
            OnSuccessCallback<ZBSendFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        void fileStatusInternal(
            bool scoring,
            std::string fileId,
            OnSuccessCallback<ZBFileStatusResponse> successCallback,
            OnErrorCallback errorCallback
        );

        void getFileInternal(
            bool scoring,
            std::string fileId,
            std::string localDownloadPath,
            OnSuccessCallback<ZBGetFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        void deleteFileInternal(
            bool scoring,
            std::string fileId,
            OnSuccessCallback<ZBDeleteFileResponse> successCallback,
            OnErrorCallback errorCallback
        );
    
    public:
        ZeroBounce();
        ZeroBounce(const ZeroBounce& obj) = delete;

        static ZeroBounce* getInstance();

        void initialize(std::string apiKey);

        void getCredits(
            OnSuccessCallback<ZBCreditsResponse> successCallback,
            OnErrorCallback errorCallback
        );

        void getApiUsage(
            std::tm startDate,
            std::tm endDate,
            OnSuccessCallback<ZBGetApiUsageResponse> successCallback,
            OnErrorCallback errorCallback
        );

        void validate(
            std::string email,
            std::string ipAddress,
            OnSuccessCallback<ZBValidateResponse> successCallback,
            OnErrorCallback errorCallback
        );

        void validateBatch(
            std::vector<ZBEmailToValidate> emailBatch,
            OnSuccessCallback<ZBValidateBatchResponse> successCallback,
            OnErrorCallback errorCallback
        );

        void sendFile(
            std::string filePath,
            int emailAddressColumnIndex,
            SendFileOptions options,
            OnSuccessCallback<ZBSendFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        void fileStatus(
            std::string fileId,
            OnSuccessCallback<ZBFileStatusResponse> successCallback,
            OnErrorCallback errorCallback
        );

        void getFile(
            std::string fileId,
            std::string localDownloadPath,
            OnSuccessCallback<ZBGetFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        void deleteFile(
            std::string fileId,
            OnSuccessCallback<ZBDeleteFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        void scoringSendFile(
            std::string filePath,
            int emailAddressColumnIndex,
            SendFileOptions options,
            OnSuccessCallback<ZBSendFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        void scoringFileStatus(
            std::string fileId,
            OnSuccessCallback<ZBFileStatusResponse> successCallback,
            OnErrorCallback errorCallback
        );

        void scoringGetFile(
            std::string fileId,
            std::string localDownloadPath,
            OnSuccessCallback<ZBGetFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        void scoringDeleteFile(
            std::string fileId,
            OnSuccessCallback<ZBDeleteFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        void getActivityData(
            std::string email,
            OnSuccessCallback<ZBActivityDataResponse> successCallback,
            OnErrorCallback errorCallback
        );
};

#endif