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
        j.at("error").get_to(r.error);
    } else {
        j.at("total").get_to(r.total);
        j.at("status_valid").get_to(r.statusValid);
        j.at("status_invalid").get_to(r.statusInvalid);
        j.at("status_catch_all").get_to(r.statusCatchAll);
        j.at("status_do_not_mail").get_to(r.statusDoNotMail);
        j.at("status_spamtrap").get_to(r.statusSpamtrap);
        j.at("status_unknown").get_to(r.statusUnknown);
        j.at("sub_status_toxic").get_to(r.subStatusToxic);
        j.at("sub_status_disposable").get_to(r.subStatusDisposable);
        j.at("sub_status_role_based").get_to(r.subStatusRoleBased);
        j.at("sub_status_possible_trap").get_to(r.subStatusPossibleTrap);
        j.at("sub_status_global_suppression").get_to(r.subStatusGlobalSuppression);
        j.at("sub_status_timeout_exceeded").get_to(r.subStatusTimeoutExceeded);
        j.at("sub_status_mail_server_temporary_error").get_to(r.subStatusMailServerTemporaryError);
        j.at("sub_status_mail_server_did_not_respond").get_to(r.subStatusMailServerDidNotRespond);
        j.at("sub_status_greylisted").get_to(r.subStatusGreyListed);
        j.at("sub_status_antispam_system").get_to(r.subStatusAntiSpamSystem);
        j.at("sub_status_does_not_accept_mail").get_to(r.subStatusDoesNotAcceptMail);
        j.at("sub_status_exception_occurred").get_to(r.subStatusExceptionOccurred);
        j.at("sub_status_failed_syntax_check").get_to(r.subStatusFailedSyntaxCheck);
        j.at("sub_status_mailbox_not_found").get_to(r.subStatusMailboxNotFound);
        j.at("sub_status_unroutable_ip_address").get_to(r.subStatusUnroutableIpAddress);
        j.at("sub_status_possible_typo").get_to(r.subStatusPossibleTypo);
        j.at("sub_status_no_dns_entries").get_to(r.subStatusNoDnsEntries);
        j.at("sub_status_role_based_catch_all").get_to(r.subStatusRoleBasedCatchAll);
        j.at("sub_status_mailbox_quota_exceeded").get_to(r.subStatusMailboxQuotaExceeded);
        j.at("sub_status_forcible_disconnect").get_to(r.subStatusForcibleDisconnect);
        j.at("sub_status_failed_smtp_connection").get_to(r.subStatusFailedSmtpConnection);
        j.at("sub_status_mx_forward").get_to(r.subStatusMxForward);
        j.at("sub_status_alternate").get_to(r.subStatusAlternate);
        j.at("sub_status_blocked").get_to(r.subStatusBlocked);
        j.at("sub_status_allowed").get_to(r.subStatusAllowed);
        j.at("start_date").get_to(r.startDate);
        j.at("end_date").get_to(r.endDate);
    }
    
    return r;
}