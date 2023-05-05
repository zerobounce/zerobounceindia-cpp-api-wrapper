#ifndef ZBSENDFILERESPONSE_H
#define ZBSENDFILERESPONSE_H

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * The model used for the POST /scoring/sendFile and POST /sendFile requests.
 */
class ZBSendFileResponse {
    public:
        bool success = false;
        std::vector<std::string> message;
        std::string fileName;
        std::string fileId;

        std::string toString();

        static ZBSendFileResponse from_json(const json& j);

        bool operator==(const ZBSendFileResponse& other) const;
};

#endif