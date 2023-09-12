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
#include "ZeroBounce/ZBFindEmailResponse.h"

#include <cpr/cpr.h>

template<typename T>
using OnSuccessCallback = std::function<void(T response)>;

using OnErrorCallback = std::function<void(ZBErrorResponse errorResponse)>;

/**
 * Struct representing the options for sending files in requests.
 */
struct SendFileOptions {
    std::string returnUrl = "";
    int firstNameColumn = 0;
    int lastNameColumn = 0;
    int genderColumn = 0;
    int ipAddressColumn = 0;
    bool hasHeaderRow = true;
    bool removeDuplicate = true;
};

/**
 * Base class for handling requests.
 */
class BaseRequestHandler {
    public:
        template <typename... Ts>
        cpr::Response Get(Ts&&... ts) {
            return doGet(std::forward<Ts>(ts)...);
        }

        template <typename... Ts>
        cpr::Response Post(Ts&&... ts) {
            return doPost(std::forward<Ts>(ts)...);
        }

    protected:
        virtual cpr::Response doGet(const cpr::Url& url) = 0;
        virtual cpr::Response doGet(const cpr::Url& url, const cpr::Header& header) = 0;
        virtual cpr::Response doPost(const cpr::Url& url, const cpr::Header& header, const cpr::Body& body) = 0;
        virtual cpr::Response doPost(const cpr::Url& url, const cpr::Header& header, const cpr::Multipart& multipart) = 0;
};

/**
 * Class used to handle http requests made with libcpr.
 */
class RequestHandler : public BaseRequestHandler {
    protected:
        cpr::Response doGet(const cpr::Url& url) {
            return cpr::Get(url);
        }
        cpr::Response doGet(const cpr::Url& url, const cpr::Header& header) {
            return cpr::Get(url, header);
        }
        cpr::Response doPost(const cpr::Url& url, const cpr::Header& header, const cpr::Body& body) {
            return cpr::Post(url, header, body);
        }
        cpr::Response doPost(const cpr::Url& url, const cpr::Header& header, const cpr::Multipart& multipart) {
            return cpr::Post(url, header, multipart);
        }
};

/**
 * The ZeroBounce main class. All the requests are implemented here.
 */
class ZeroBounce {
    protected:
        BaseRequestHandler* requestHandler = new RequestHandler();
    private:
        static ZeroBounce* instance;
        std::string apiKey;
        const std::string apiBaseUrl = "https://api.zerobounce.in/v2";
        const std::string bulkApiBaseUrl = "https://bulkapi.zerobounce.in/v2";
        const std::string bulkApiScoringBaseUrl = "https://bulkapi.zerobounce.in/v2/scoring";

        /**
         * Checks if the [apiKey] is invalid or not and if it is, then it throws an error through the provided
         * [errorCallback].
         *
         * @param errorCallback the error callback
         * @return **true** if the [apiKey] is null or **false** otherwise
         */
        bool invalidApiKey(OnErrorCallback errorCallback);

        /**
         * The helper method that handles GET requests.
         *
         * @tparam T              type of the response
         * @param urlPath         the url
         * @param successCallback the success callback
         * @param errorCallback   the error callback
         */
        template <typename T>
        void sendRequest(
            std::string urlPath,
            OnSuccessCallback<T> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * The sendFile API allows user to send a file for bulk email validation. This method implements the actual
         * request logic.
         *
         * @param scoring                 *true* if the AI scoring should be used, or *false* otherwise
         * @param filePath                the path of the file to send
         * @param emailAddressColumnIndex the column index of the email address in the file. Index starts from 1
         * @param options                 the send file options
         * @param successCallback         the success callback
         * @param errorCallback           the error callback
         */
        void sendFileInternal(
            bool scoring,
            std::string filePath,
            int emailAddressColumnIndex,
            SendFileOptions options,
            OnSuccessCallback<ZBSendFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * Returns the status of a file submitted for email validation. This method implements the actual request logic.
         *
         * @param scoring         *true* if the AI scoring should be used, or *false* otherwise
         * @param fileId          the returned file ID when calling either the sendFile or scoringSendFile APIs
         * @param successCallback the success callback
         * @param errorCallback   the error callback
         */
        void fileStatusInternal(
            bool scoring,
            std::string fileId,
            OnSuccessCallback<ZBFileStatusResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * The getFile API allows users to get the validation results file for the file that has  been submitted.
         * This method implements the actual request logic.
         *
         * @param scoring           *true* if the AI scoring should be used, or *false* otherwise
         * @param fileId            the returned file ID when calling sendFile API
         * @param localDownloadPath the path to which to download the file
         * @param successCallback   the success callback
         * @param errorCallback     the error callback
         */
        void getFileInternal(
            bool scoring,
            std::string fileId,
            std::string localDownloadPath,
            OnSuccessCallback<ZBGetFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * Delete a file. This method implements the actual request logic.
         *
         * @param scoring         *true* if the AI scoring should be used, or *false* otherwise
         * @param fileId          the returned file ID when calling sendFile API
         * @param successCallback the success callback
         * @param errorCallback   the error callback
         */
        void deleteFileInternal(
            bool scoring,
            std::string fileId,
            OnSuccessCallback<ZBDeleteFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

    public:
        ZeroBounce();
        ZeroBounce(const ZeroBounce& obj) = delete;

        /**
         * Get pointer to ZeroBounce instance.
         *
         * @return ZeroBounce*
         */
        static ZeroBounce* getInstance();

        /**
         * Initializes the SDK.
         *
         * @param apiKey the API key
         */
        void initialize(std::string apiKey);

        /**
         * This API will tell you how many credits you have left on your account.
         *
         * @param successCallback the success callback
         * @param errorCallback   the error callback
         */
        void getCredits(
            OnSuccessCallback<ZBCreditsResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * Returns the API usage between the given dates.
         *
         * @param startDate       the start date of when you want to view API usage
         * @param endDate         the end date of when you want to view API usage
         * @param successCallback the success callback
         * @param errorCallback   the error callback
         */
        void getApiUsage(
            std::tm startDate,
            std::tm endDate,
            OnSuccessCallback<ZBGetApiUsageResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * Validates the given email.
         *
         * @param email           the email address you want to validate
         * @param ipAddress       the IP Address the email signed up from (Can be blank)
         * @param successCallback the success callback
         * @param errorCallback   the error callback
         */
        void validate(
            std::string email,
            std::string ipAddress,
            OnSuccessCallback<ZBValidateResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * Validates a batch of emails.
         *
         * @param emailBatch      the ZBEmailToValidate items you want to validate
         * @param successCallback the success callback
         * @param errorCallback   the error callback
         */
        void validateBatch(
            std::vector<ZBEmailToValidate> emailBatch,
            OnSuccessCallback<ZBValidateBatchResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * The sendFile API allows user to send a file for bulk email validation.
         *
         * @param fifilePathle            the path of the file to send
         * @param emailAddressColumnIndex the column index of the email address in the file. Index starts from 1.
         * @param options                 the send file options
         * @param successCallback         the success callback
         * @param errorCallback           the error callback
         */
        void sendFile(
            std::string filePath,
            int emailAddressColumnIndex,
            SendFileOptions options,
            OnSuccessCallback<ZBSendFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * Returns the status of a file submitted for email validation.
         *
         * @param fileId          the returned file ID when calling sendFile API
         * @param successCallback the success callback
         * @param errorCallback   the error callback
         */
        void fileStatus(
            std::string fileId,
            OnSuccessCallback<ZBFileStatusResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * The getFile API allows users to get the validation results file for the file been submitted using sendFile API.
         *
         * @param fileId               the returned file ID when calling sendFile API
         * @param localDownloadPath    the path to which to download the file
         * @param successCallback      the success callback
         * @param errorCallback        the error callback
         */
        void getFile(
            std::string fileId,
            std::string localDownloadPath,
            OnSuccessCallback<ZBGetFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * Delete a file.
         *
         * @param fileId          the returned file ID when calling sendFile API
         * @param successCallback the success callback
         * @param errorCallback   the error callback
         */
        void deleteFile(
            std::string fileId,
            OnSuccessCallback<ZBDeleteFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * The scoringSendFile API allows user to send a file for bulk email validation.
         *
         * @param filePath                the path of the file to send
         * @param emailAddressColumnIndex the column index of the email address in the file. Index starts from 1.
         * @param options                 the send file options
         * @param successCallback         the success callback
         * @param errorCallback           the error callback
         */
        void scoringSendFile(
            std::string filePath,
            int emailAddressColumnIndex,
            SendFileOptions options,
            OnSuccessCallback<ZBSendFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * Returns the status of a file submitted for email validation using the AI Scoring request.
         *
         * @param fileId          the returned file ID when calling scoringSendFile API
         * @param successCallback the success callback
         * @param errorCallback   the error callback
         */
        void scoringFileStatus(
            std::string fileId,
            OnSuccessCallback<ZBFileStatusResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * The scoringGetFile API allows users to get the validation results file for the file been submitted using
         * scoringSendFile API.
         *
         * @param fileId            the returned file ID when calling sendFile API
         * @param localDownloadPath the path to which to download the file
         * @param successCallback   the success callback
         * @param errorCallback     the error callback
         */
        void scoringGetFile(
            std::string fileId,
            std::string localDownloadPath,
            OnSuccessCallback<ZBGetFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * Delete a file submitted using the scoring API.
         *
         * @param fileId          the returned file ID when calling sendFile API
         * @param successCallback the success callback
         * @param errorCallback   the error callback
         */
        void scoringDeleteFile(
            std::string fileId,
            OnSuccessCallback<ZBDeleteFileResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * The request returns data regarding opens, clicks, forwards and unsubscribes that have taken place in the past
         * 30, 90, 180 or 365 days.
         *
         * @param email           the email address
         * @param successCallback the success callback
         * @param errorCallback   the error callback
         */
        void getActivityData(
            std::string email,
            OnSuccessCallback<ZBActivityDataResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * @brief Email Address Search - Identifies and validates a person’s primary email address
         *
         * @param zb                ZeroBounce pointer
         * @param domain            The email domain for which to find the email format
         * @param first_name        The first name of the person whose email format is being searched
         * @param middle_name       The middle name of the person whose email format is being searched
         * @param last_name         The last name of the person whose email format is being searched
         * @param success_callback  success callback
         * @param error_callback    error callback
         */
        void findEmail(
            std::string domain,
            std::string first_name,
            std::string middle_name,
            std::string last_name,
            OnSuccessCallback<ZBFindEmailResponse> successCallback,
            OnErrorCallback errorCallback
        );

        /**
         * @brief Email Address Search - Identifies and validates a person’s primary email address
         *
         * @param zb                ZeroBounce pointer
         * @param domain            The email domain for which to find the email format
         * @param first_name        The first name of the person whose email format is being searched
         * @param last_name         The last name of the person whose email format is being searched
         * @param success_callback  success callback
         * @param error_callback    error callback
         */
        void findEmail(
            std::string domain,
            std::string first_name,
            std::string last_name,
            OnSuccessCallback<ZBFindEmailResponse> successCallback,
            OnErrorCallback errorCallback
        );
};

#endif