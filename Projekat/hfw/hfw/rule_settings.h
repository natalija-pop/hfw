#pragma once
#include "fw_rule.h"
#include <atlcomcli.h>

void CreateNewRule(const FwRule& fwNewRule);
void AddNewRule();
void UpdateFwRuleChanges(CComPtr<INetFwRule> fwRule, const FwRule& updatedFwRule);
void UpdateFwRule(CComPtr<INetFwRule> fwRule, int position);
void UpdatePropertiesDialog(CComPtr<INetFwRule> fwRule);
void SetFwRuleInitialValues(CComPtr<INetFwRule> fwRule, FwRule& updatedRule);