#ifndef ZBGETFILERESPONSE_H
#define ZBGETFILERESPONSE_H

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ZBGetFileResponse {
    public:
        bool success = false;
        std::string message;
        std::string fileName;
        std::string localFilePath;

        std::string toString();

        static ZBGetFileResponse from_json(const json& j);
};

#endif