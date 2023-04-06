#ifndef ZBDELETEFILERESPONSE_H
#define ZBDELETEFILERESPONSE_H

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * The model class for the GET /scoring/deletefile GET /deletefile requests.
 */
class ZBDeleteFileResponse {
    public:
        bool success = false;
        std::string message;
        std::string fileName;
        std::string fileId;

        std::string toString();

        static ZBDeleteFileResponse from_json(const json& j);

        bool operator==(const ZBDeleteFileResponse& other) const;
};

#endif