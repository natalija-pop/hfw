#pragma once
#include <icftypes.h>


class FwProfileManager
{
	NET_FW_PROFILE_TYPE2 profileType;

	NET_FW_ACTION defaultInboundAction;
	NET_FW_ACTION defaultOutboundAction;

	VARIANT_BOOL bIsFwEnabled;
	VARIANT_BOOL bIsInboundEnabled;
	VARIANT_BOOL bAreNotificationsDisabled;
	VARIANT_BOOL bIsUnicastResponseDisabled;


public:
	FwProfileManager();

	FwProfileManager(NET_FW_PROFILE_TYPE2 newProfileType, NET_FW_ACTION newDefaultInboundAction, NET_FW_ACTION newDefaultOutboundAction,
		VARIANT_BOOL newIsFwEnabled, VARIANT_BOOL newIsInboundEnabled, VARIANT_BOOL newAreNotificationsEnabled, VARIANT_BOOL newIsUnicastEnabled);

	NET_FW_PROFILE_TYPE2 get_ProfileType();
	NET_FW_ACTION get_DefaultInboundAction();
	NET_FW_ACTION get_DefaultOutboundAction();

	VARIANT_BOOL get_IsFwEnabled();
	VARIANT_BOOL get_IsInboundEnabled();
	VARIANT_BOOL get_AreNotificationsDisabled();
	VARIANT_BOOL get_IsUnicastResponseDIsabled();
};