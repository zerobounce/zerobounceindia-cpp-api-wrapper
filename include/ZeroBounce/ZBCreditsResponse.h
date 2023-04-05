#ifndef ZBCREDITSRESPONSE_H
#define ZBCREDITSRESPONSE_H

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ZBCreditsResponse {
    public:
        int credits = -1;

        std::string toString();

        static ZBCreditsResponse from_json(const json& j);

        bool operator==(const ZBCreditsResponse& other) const;
};

#endif