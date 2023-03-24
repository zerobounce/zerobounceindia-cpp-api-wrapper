#include <sstream>

#include "ZeroBounce/ZBValidateResponse.h"

std::string ZBValidateResponse::toString(bool isBatch)
{
    std::stringstream stringStream;
    stringStream << "ZBValidateResponse{" <<
                "address='" << address << '\'' <<
                ", status=" << status <<
                ", subStatus=" << subStatus <<
                ", freeEmail=" << freeEmail <<
                ", didYouMean='" << didYouMean << '\'' <<
                ", account='" << account << '\'' <<
                ", domain='" << domain << '\'' <<
                ", domainAgeDays='" << domainAgeDays << '\'' <<
                ", smtpProvider='" << smtpProvider << '\'' <<
                ", mxFound=" << mxFound <<
                ", mxRecord='" << mxRecord << '\'' <<
                ", firstName='" << firstName << '\'' <<
                ", lastName='" << lastName << '\'' <<
                ", gender='" << gender << '\'' <<
                ", country='" << country << '\'' <<
                ", region='" << region << '\'' <<
                ", city='" << city << '\'' <<
                ", zipCode='" << zipCode << '\'' <<
                ", processedAt='" << processedAt << '\'';

    if (!isBatch) {
        stringStream << ", error='" << error << '\'';
    }

    stringStream << '}';

    return stringStream.str();
}

ZBValidateResponse ZBValidateResponse::from_json(const json& j) {
    ZBValidateResponse r;

    if (j.find("error") != j.end()) {
        j.at("error").get_to(r.error);
    } else {
        r.address = j.at("address").get<std::string>();
        r.status = j.at("status").get<ZBValidateStatus>();
        r.subStatus = j.at("sub_status").get<ZBValidateSubStatus>();
        r.freeEmail = j.at("free_email").get<bool>();
        r.didYouMean = j.at("did_you_mean").is_null() ?
            "" : j.at("did_you_mean").get<std::string>();
        r.account = j.at("account").get<std::string>();
        r.domain = j.at("domain").get<std::string>();
        r.domainAgeDays = j.at("domain_age_days").is_null() ?
            "" : j.at("domain_age_days").get<std::string>();
        r.smtpProvider = j.at("smtp_provider").is_null() ?
            "" : j.at("smtp_provider").get<std::string>();
        
        bool mxFound;
        std::istringstream ss(j.at("mx_found").get<std::string>());
        ss >> std::boolalpha >> mxFound;
        r.mxFound = mxFound;

        r.mxRecord = j.at("mx_record").get<std::string>();
        r.firstName = j.at("firstname").is_null() ?
            "" : j.at("firstname").get<std::string>();
        r.lastName = j.at("lastname").is_null() ?
            "" : j.at("lastname").get<std::string>();
        r.gender = j.at("gender").is_null() ?
            "" : j.at("gender").get<std::string>();
        r.country = j.at("country").is_null() ?
            "" : j.at("country").get<std::string>();
        r.region = j.at("region").is_null() ?
            "" : j.at("region").get<std::string>();
        r.city = j.at("city").is_null() ?
            "" : j.at("city").get<std::string>();
        r.zipCode = j.at("zipcode").is_null() ?
            "" : j.at("zipcode").get<std::string>();
        j.at("processed_at").get_to(r.processedAt);
    }
    
    return r;
}