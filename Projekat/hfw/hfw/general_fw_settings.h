#pragma once
#include <icftypes.h>
#include <netfw.h>

#include "fw_profile_manager.h"

void DisableWindowsFirewall();
void ManageProfileState(NET_FW_PROFILE_TYPE2 profileType, bool enabled);
void ManageProfileFwStateMenu(bool enabled);
void GetFirewallSettings();
void GetFirewallSettingsPerProfile(NET_FW_PROFILE_TYPE2 profileType, INetFwPolicy2* pNetFwPolicy2);
void UpdateSettingsPerProfile(INetFwPolicy2* pNetFwPolicy2);
void UpdateProfile(FwProfileManager fwProfileManager, INetFwPolicy2* pNetFwPolicy2);