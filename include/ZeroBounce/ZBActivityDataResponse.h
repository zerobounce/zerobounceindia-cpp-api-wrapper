#ifndef ZBACTIVITYDATARESPONSE_H
#define ZBACTIVITYDATARESPONSE_H

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * The model used for the GET /activity request.
 */
class ZBActivityDataResponse {
    public:
        bool found = false;
        int activeInDays = -1;
        std::string error;

        std::string toString();

        static ZBActivityDataResponse from_json(const json& j);

        bool operator==(const ZBActivityDataResponse& other) const;
};

#endif