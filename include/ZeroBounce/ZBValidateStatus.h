#ifndef ZBVALIDATESTATUS_H
#define ZBVALIDATESTATUS_H

#include "nlohmann/json.hpp"

/**
 * Enum that lists all the possible statuses of the email validation result.
 */
enum ZBValidateStatus {
    UnknownValue,
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
        {ZBValidateStatus::UnknownValue, "unknown"},
        {ZBValidateStatus::Valid, "valid"},
        {ZBValidateStatus::Invalid, "invalid"},
        {ZBValidateStatus::CatchAll, "catch-all"},
        {ZBValidateStatus::Spamtrap, "spamtrap"},
        {ZBValidateStatus::Abuse, "abuse"},
        {ZBValidateStatus::DoNotMail, "do_not_mail"}
    }
)

#endif