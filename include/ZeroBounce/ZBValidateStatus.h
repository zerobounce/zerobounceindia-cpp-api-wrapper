#ifndef ZBVALIDATESTATUS_H
#define ZBVALIDATESTATUS_H

#include "nlohmann/json.hpp"

enum ZBValidateStatus {
    Valid,
    Invalid,
    CatchAll,
    Unknown,
    Spamtrap,
    Abuse,
    DoNotMail
};

NLOHMANN_JSON_SERIALIZE_ENUM(
    ZBValidateStatus,
    {
        {ZBValidateStatus::Valid, "valid"},
        {ZBValidateStatus::Invalid, "invalid"},
        {ZBValidateStatus::CatchAll, "catch-all"},
        {ZBValidateStatus::Unknown, "unknown"},
        {ZBValidateStatus::Spamtrap, "spamtrap"},
        {ZBValidateStatus::Abuse, "abuse"},
        {ZBValidateStatus::DoNotMail, "do_not_mail"}
    }
)

#endif