#include <nlohmann/json.hpp>

#include "ZeroBounce/ZBErrorResponse.h"

using json = nlohmann::json;

ZBErrorResponse::ZBErrorResponse() {}

ZBErrorResponse::ZBErrorResponse(std::optional<bool> success, const std::vector<std::string>& errors) :
        success(success), errors(errors) {}

ZBErrorResponse ZBErrorResponse::parseError(const std::string& error) {
    ZBErrorResponse response;

    if (error.empty()) {
        return response;
    }

    std::vector<std::string> errors;
    std::vector<std::string> otherMessages;

    std::unordered_map<std::string, nlohmann::json> hashMap = json::parse(error);

    for (auto& [key, value] : hashMap) {
        if (key.find("error") != std::string::npos || key.find("message") != std::string::npos) {
            if (value.is_array()) {
                for (auto& error : value) {
                    errors.push_back(error.get<std::string>());
                }
            } else if (!value.is_null()) {
                errors.push_back(value.get<std::string>());
            }
        } else {
            if (key == "success" && value.is_boolean()) {
                response.success = value.get<bool>();
            } else if (value.is_array()) {
                for (auto& message : value) {
                    otherMessages.push_back(message.get<std::string>());
                }
            } else if (!value.is_null()) {
                otherMessages.push_back(value.get<std::string>());
            }
        }
    }

    errors.insert(errors.end(), otherMessages.begin(), otherMessages.end());

    response.errors = errors;
    return response;
}