#include <sstream>

#include "ZeroBounce/ZBGetApiUsageResponse.h"

std::string ZBGetApiUsageResponse::toString()
{
    std::stringstream stringStream;
    stringStream << "ZBGetApiUsageResponse{" <<
                "total=" << total <<
                ", statusValid=" << statusValid <<
                ", statusInvalid=" << statusInvalid <<
                ", statusCatchAll=" << statusCatchAll <<
                ", statusDoNotMail=" << statusDoNotMail <<
                ", statusSpamtrap=" << statusSpamtrap <<
                ", statusUnknown=" << statusUnknown <<
                ", subStatusToxic=" << subStatusToxic <<
                ", subStatusDisposable=" << subStatusDisposable <<
                ", subStatusRoleBased=" << subStatusRoleBased <<
                ", subStatusPossibleTrap=" << subStatusPossibleTrap <<
                ", subStatusGlobalSuppression=" << subStatusGlobalSuppression <<
                ", subStatusTimeoutExceeded=" << subStatusTimeoutExceeded <<
                ", subStatusMailServerTemporaryError=" << subStatusMailServerTemporaryError <<
                ", subStatusMailServerDidNotRespond=" << subStatusMailServerDidNotRespond <<
                ", subStatusGreyListed=" << subStatusGreyListed <<
                ", subStatusAntiSpamSystem=" << subStatusAntiSpamSystem <<
                ", subStatusDoesNotAcceptMail=" << subStatusDoesNotAcceptMail <<
                ", subStatusExceptionOccurred=" << subStatusExceptionOccurred <<
                ", subStatusFailedSyntaxCheck=" << subStatusFailedSyntaxCheck <<
                ", subStatusMailboxNotFound=" << subStatusMailboxNotFound <<
                ", subStatusUnRoutableIpAddress=" << subStatusUnroutableIpAddress <<
                ", subStatusPossibleTypo=" << subStatusPossibleTypo <<
                ", subStatusNoDnsEntries=" << subStatusNoDnsEntries <<
                ", subStatusRoleBasedCatchAll=" << subStatusRoleBasedCatchAll <<
                ", subStatusMailboxQuotaExceeded=" << subStatusMailboxQuotaExceeded <<
                ", subStatusForcibleDisconnect=" << subStatusForcibleDisconnect <<
                ", subStatusFailedSmtpConnection=" << subStatusFailedSmtpConnection <<
                ", startDate='" << startDate << '\'' <<
                ", endDate='" << endDate << '\'' <<
                ", error='" << error << '\'' <<
                '}';
        
    return stringStream.str();
}

ZBGetApiUsageResponse ZBGetApiUsageResponse::from_json(const json& j) {
    ZBGetApiUsageResponse r;

    if (j.find("error") != j.end()) {
        r.error = j.at("error").get<std::string>();
    } else {
        r.total = j.at("total").get<int>();
        r.statusValid = j.at("status_valid").get<int>();
        r.statusInvalid = j.at("status_invalid").get<int>();
        r.statusCatchAll = j.at("status_catch_all").get<int>();
        r.statusDoNotMail = j.at("status_do_not_mail").get<int>();
        r.statusSpamtrap = j.at("status_spamtrap").get<int>();
        r.statusUnknown = j.at("status_unknown").get<int>();
        r.subStatusToxic = j.at("sub_status_toxic").get<int>();
        r.subStatusDisposable = j.at("sub_status_disposable").get<int>();
        r.subStatusRoleBased = j.at("sub_status_role_based").get<int>();
        r.subStatusPossibleTrap = j.at("sub_status_possible_trap").get<int>();
        r.subStatusGlobalSuppression = j.at("sub_status_global_suppression").get<int>();
        r.subStatusTimeoutExceeded = j.at("sub_status_timeout_exceeded").get<int>();
        r.subStatusMailServerTemporaryError = j.at("sub_status_mail_server_temporary_error").get<int>();
        r.subStatusMailServerDidNotRespond = j.at("sub_status_mail_server_did_not_respond").get<int>();
        r.subStatusGreyListed = j.at("sub_status_greylisted").get<int>();
        r.subStatusAntiSpamSystem = j.at("sub_status_antispam_system").get<int>();
        r.subStatusDoesNotAcceptMail = j.at("sub_status_does_not_accept_mail").get<int>();
        r.subStatusExceptionOccurred = j.at("sub_status_exception_occurred").get<int>();
        r.subStatusFailedSyntaxCheck = j.at("sub_status_failed_syntax_check").get<int>();
        r.subStatusMailboxNotFound = j.at("sub_status_mailbox_not_found").get<int>();
        r.subStatusUnroutableIpAddress = j.at("sub_status_unroutable_ip_address").get<int>();
        r.subStatusPossibleTypo = j.at("sub_status_possible_typo").get<int>();
        r.subStatusNoDnsEntries = j.at("sub_status_no_dns_entries").get<int>();
        r.subStatusRoleBasedCatchAll = j.at("sub_status_role_based_catch_all").get<int>();
        r.subStatusMailboxQuotaExceeded = j.at("sub_status_mailbox_quota_exceeded").get<int>();
        r.subStatusForcibleDisconnect = j.at("sub_status_forcible_disconnect").get<int>();
        r.subStatusFailedSmtpConnection = j.at("sub_status_failed_smtp_connection").get<int>();
        r.subStatusMxForward = j.at("sub_status_mx_forward").get<int>();
        r.subStatusAlternate = j.at("sub_status_alternate").get<int>();
        r.subStatusBlocked = j.at("sub_status_blocked").get<int>();
        r.subStatusAllowed = j.at("sub_status_allowed").get<int>();
        r.startDate = j.at("start_date").get<std::string>();
        r.endDate = j.at("end_date").get<std::string>();
    }
    
    return r;
}