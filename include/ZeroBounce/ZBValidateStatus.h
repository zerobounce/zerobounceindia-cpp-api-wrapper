#ifndef ZBVALIDATESTATUS_H
#define ZBVALIDATESTATUS_H

enum ZBValidateStatus {
    Valid,
    Invalid,
    CatchAll,
    Unknown,
    Spamtrap,
    Abuse,
    DoNotMail
};

#endif