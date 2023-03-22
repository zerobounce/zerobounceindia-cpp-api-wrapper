#ifndef ZBERRORRESPONSE_H
#define ZBERRORRESPONSE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

class ZBErrorResponse {
    public:
        std::optional<bool> success;
        std::vector<std::string> errors;

        ZBErrorResponse();
        ZBErrorResponse(std::optional<bool> success, const std::vector<std::string>& errors);

        static ZBErrorResponse parseError(const std::string& error);
};

#endif