#pragma once
#include <atlcomcli.h>
#include <netfw.h>
#include <vector>

void DumpFWRulesInCollection(INetFwRule* fwRule, int position);
void EnumerateFWRules(NET_FW_RULE_DIRECTION fwDirection); //In or Out
void EnumerateFwRulesMenu(NET_FW_RULE_DIRECTION fwDirection);
bool ChangeFwRuleEnabled(std::vector<CComPtr<INetFwRule>>& fwRulesVec, int position);
bool RemoveFwRule(INetFwRules* fwRules, std::vector<CComPtr<INetFwRule>>& fwRulesVec, int position);
void PrintLocalizableString(PCWSTR label, PCWSTR value);