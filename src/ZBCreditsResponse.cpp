#include <sstream>

#include "ZeroBounce/ZBCreditsResponse.h"

std::string ZBCreditsResponse::toString()
{
    std::stringstream stringStream;
    stringStream << "ZBCreditsResponse{" <<
                "credits='" << credits << '\'' <<
                '}';
        
    return stringStream.str();
}

ZBCreditsResponse ZBCreditsResponse::from_json(const json& j) {
    ZBCreditsResponse r;
    r.credits = j.at("Credits").get<std::string>();
    return r;
}