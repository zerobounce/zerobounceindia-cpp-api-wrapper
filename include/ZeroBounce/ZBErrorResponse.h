#ifndef ZBERRORRESPONSE_H
#define ZBERRORRESPONSE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

class ZBErrorResponse {
    public:
        bool success = false;
        std::vector<std::string> errors;

        ZBErrorResponse();
        ZBErrorResponse(bool success, std::vector<std::string> errors);

        std::string toString();

        static ZBErrorResponse parseError(std::string error);
};

#endif