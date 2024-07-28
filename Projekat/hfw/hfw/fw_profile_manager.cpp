#include "fw_profile_manager.h"

FwProfileManager::FwProfileManager() :
	profileType(NET_FW_PROFILE2_DOMAIN), defaultInboundAction(NET_FW_ACTION_BLOCK),	defaultOutboundAction(NET_FW_ACTION_ALLOW),	bIsFwEnabled(VARIANT_FALSE),
	bIsInboundEnabled(VARIANT_FALSE), bAreNotificationsDisabled(VARIANT_FALSE), bIsUnicastResponseDisabled(VARIANT_FALSE)
{

}

FwProfileManager::FwProfileManager(NET_FW_PROFILE_TYPE2 newProfileType, NET_FW_ACTION newDefaultInboundAction,
	NET_FW_ACTION newDefaultOutboundAction, VARIANT_BOOL newIsFwEnabled, VARIANT_BOOL newIsInboundEnabled,
	VARIANT_BOOL newAreNotificationsEnabled, VARIANT_BOOL newIsUnicastEnabled) :
	profileType(newProfileType), defaultInboundAction(newDefaultInboundAction), defaultOutboundAction(newDefaultOutboundAction),
	bIsFwEnabled(newIsFwEnabled), bIsInboundEnabled(newIsInboundEnabled), bAreNotificationsDisabled(newAreNotificationsEnabled), bIsUnicastResponseDisabled(newIsUnicastEnabled)
{
	
}

NET_FW_PROFILE_TYPE2 FwProfileManager::get_ProfileType()
{
	return profileType;
}

NET_FW_ACTION FwProfileManager::get_DefaultInboundAction()
{
	return defaultInboundAction;
}

NET_FW_ACTION FwProfileManager::get_DefaultOutboundAction()
{
	return  defaultOutboundAction;
}

VARIANT_BOOL FwProfileManager::get_IsFwEnabled()
{
	return bIsFwEnabled;
}

VARIANT_BOOL FwProfileManager::get_IsInboundEnabled()
{
	return bIsInboundEnabled;
}

VARIANT_BOOL FwProfileManager::get_AreNotificationsDisabled()
{
	return bAreNotificationsDisabled;
}

VARIANT_BOOL FwProfileManager::get_IsUnicastResponseDIsabled()
{
	return bIsUnicastResponseDisabled;
}