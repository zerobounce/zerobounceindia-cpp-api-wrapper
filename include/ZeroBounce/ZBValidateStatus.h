#ifndef ZBVALIDATESTATUS_H
#define ZBVALIDATESTATUS_H

#include "nlohmann/json.hpp"

enum ZBValidateStatus {
    Unknown,
    Valid,
    Invalid,
    CatchAll,
    Spamtrap,
    Abuse,
    DoNotMail
};

NLOHMANN_JSON_SERIALIZE_ENUM(
    ZBValidateStatus,
    {
        {ZBValidateStatus::Unknown, "unknown"},
        {ZBValidateStatus::Valid, "valid"},
        {ZBValidateStatus::Invalid, "invalid"},
        {ZBValidateStatus::CatchAll, "catch-all"},
        {ZBValidateStatus::Spamtrap, "spamtrap"},
        {ZBValidateStatus::Abuse, "abuse"},
        {ZBValidateStatus::DoNotMail, "do_not_mail"}
    }
)

#endif