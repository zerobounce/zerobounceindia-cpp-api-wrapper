#include <sstream>

#include "ZeroBounce/utils.h"
#include "ZeroBounce/ZBValidateBatchResponse.h"

std::string ZBValidateError::toString()
{
    std::stringstream stringStream;
    stringStream << "{error='" << error << '\'' <<
                ", emailAddress='" << emailAddress << "\'}";

    return stringStream.str();
}

ZBValidateError ZBValidateError::from_json(const json& j) {
    ZBValidateError r;

    r.error = getOrDefault<std::string>(j, "error", "");
    r.emailAddress = getOrDefault<std::string>(j, "email_address", "");

    return r;
}

bool ZBValidateError::operator==(const ZBValidateError& other) const {
    return error == other.error &&
        emailAddress == other.emailAddress;
}

std::string ZBValidateBatchResponse::toString()
{
    std::stringstream stringStream;
    stringStream << "ZBValidateBatchResponse{" << "emailBatch=[";

    for (auto it = emailBatch.begin(); it != emailBatch.end(); ++it) {
        stringStream << it->toString(true);
        if (std::next(it) != emailBatch.end()) {
            stringStream << ", ";
        }
    }

    stringStream << "], errors=[";

    for (auto it = errors.begin(); it != errors.end(); ++it) {
        stringStream << it->toString();
        if (std::next(it) != errors.end()) {
            stringStream << ", ";
        }
    }

    stringStream << "]}";

    return stringStream.str();
}

ZBValidateBatchResponse ZBValidateBatchResponse::from_json(const json& j) {
    ZBValidateBatchResponse r;

    for (auto& emailObj : j.at("email_batch")) {
        ZBValidateResponse emailResponse = ZBValidateResponse::from_json(emailObj);
        r.emailBatch.push_back(emailResponse);
    }

    for (auto& errorObj : j.at("errors")) {
        ZBValidateError error = ZBValidateError::from_json(errorObj);
        r.errors.push_back(error);
    }

    return r;
}

bool ZBValidateBatchResponse::operator==(const ZBValidateBatchResponse& other) const {
    return emailBatch == other.emailBatch &&
        errors == other.errors;
}