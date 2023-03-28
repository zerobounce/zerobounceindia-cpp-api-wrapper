#include <sstream>

#include "ZeroBounce/utils.h"
#include "ZeroBounce/ZBActivityDataResponse.h"

std::string ZBActivityDataResponse::toString()
{
    std::stringstream stringStream;
    stringStream << "ZBActivityDataResponse{" <<
                "found=" << found <<
                ", activeInDays='" << activeInDays << '\'' <<
                '}';
        
    return stringStream.str();
}

ZBActivityDataResponse ZBActivityDataResponse::from_json(const json& j) {
    ZBActivityDataResponse r;

    r.found = getOrDefault<bool>(j, "found", false);
    r.activeInDays = std::stoi(getOrDefault<std::string>(j, "active_in_days", "-1"));

    return r;
}