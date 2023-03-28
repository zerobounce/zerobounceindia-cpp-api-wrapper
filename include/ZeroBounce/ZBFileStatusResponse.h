#ifndef ZBFILESTATUSRESPONSE_H
#define ZBFILESTATUSRESPONSE_H

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ZBFileStatusResponse {
    public:
        bool success = false;
        std::string message;
        std::string errorReason;
        std::string fileId;
        std::string fileName;
        std::string uploadDate;
        std::string fileStatus;
        std::string completePercentage;
        std::string returnUrl;

        std::string toString();

        static ZBFileStatusResponse from_json(const json& j);
};

#endif