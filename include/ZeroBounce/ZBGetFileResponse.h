#ifndef ZBGETFILERESPONSE_H
#define ZBGETFILERESPONSE_H

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * The model used for the GET /scoring/getFile and GET /getFile requests.
 */
class ZBGetFileResponse {
    public:
        bool success = false;
        std::string message;
        std::string localFilePath;

        std::string toString();

        static ZBGetFileResponse from_json(const json& j);

        bool operator==(const ZBGetFileResponse& other) const;
};

#endif