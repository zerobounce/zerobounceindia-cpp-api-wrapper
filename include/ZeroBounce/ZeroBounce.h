#ifndef ZEROBOUNCE_H
#define ZEROBOUNCE_H

#include <functional>
#include <string>

#include "ZeroBounce/ZBErrorResponse.h"
#include "ZeroBounce/ZBCreditsResponse.h"

class ZeroBounce {
    private:
        static ZeroBounce* instance;
        std::string apiKey;
        const std::string apiBaseUrl = "https://api.zerobounce.net/v2";
        const std::string bulkApiBaseUrl = "https://bulkapi.zerobounce.net/v2";
        const std::string bulkApiScoringBaseUrl = "https://bulkapi.zerobounce.net/v2/scoring";
    
    public:
        template<typename T>
        using OnSuccessCallback = std::function<void(T response)>;

        using OnErrorCallback = std::function<void(ZBErrorResponse errorResponse)>;

        ZeroBounce();
        ZeroBounce(const ZeroBounce& obj) = delete;

        static ZeroBounce* getInstance();
        void initialize(std::string apiKey);
        bool invalidApiKey(OnErrorCallback errorCallback);


        template <typename T>
        void sendRequest(
            std::string urlPath,
            OnSuccessCallback<T> successCallback,
            OnErrorCallback errorCallback
        );

        void getCredits(
            OnSuccessCallback<ZBCreditsResponse> successCallback,
            OnErrorCallback errorCallback
        );

};

class FileRequestHandler {
    public:
        void sendFile(
            std::string file, int emailAddressColumn=1, int firstNameColumn=2, int lastNameColumn=3, int genderColumn=4,
            bool hasHeader=false
        );
        void fileStatus(std::string fileId);
        void getFile(std::string fileId);
        void deleteFile(std::string fileId);
};

#endif