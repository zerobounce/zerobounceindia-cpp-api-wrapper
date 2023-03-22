#ifndef ZBCREDITSRESPONSE_H
#define ZBCREDITSRESPONSE_H

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ZBCreditsResponse {
    public:
        std::string credits;

        std::string toString();

        static ZBCreditsResponse from_json(const json& j);
};

#endif