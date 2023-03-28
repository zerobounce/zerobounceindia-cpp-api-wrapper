#ifndef ZBDELETEFILERESPONSE_H
#define ZBDELETEFILERESPONSE_H

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ZBDeleteFileResponse {
    public:
        bool success = false;
        std::string message;
        std::string fileName;
        std::string fileId;

        std::string toString();

        static ZBDeleteFileResponse from_json(const json& j);
};

#endif