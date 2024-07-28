/********************************************************************++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) Microsoft Corporation. All Rights Reserved.

Abstract:
    This C++ file includes sample code for disabling Windows Firewall
    per profile using the Microsoft Windows Firewall APIs.

--********************************************************************/

#include  "general_fw_settings.h"

#include <cstdio>
#include <iostream>
#include "helper.h"

using namespace std;

VARIANT_BOOL getYesNoInput(const string& prompt) {
    CHAR cInput;

    do {
        cout << prompt << " (y/n)\n";
        cin >> cInput;
        if (cInput == 'y') return true;
        if (cInput == 'n') return false;
        cout << "Wrong input. Try again.\n";

    } while (true);
}

NET_FW_PROFILE_TYPE2 getProfileType() {
    int iProfileType;

    do {
        cout << "Select profile you want to change:\n"
            << "1. Domain\n2. Private\n3. Public\n";
        cin >> iProfileType;
        switch (iProfileType) {
        case 1: return NET_FW_PROFILE2_DOMAIN;
        case 2: return NET_FW_PROFILE2_PRIVATE;
        case 3: return NET_FW_PROFILE2_PUBLIC;
        default: cout << "Wrong input. Try again.\n";
        }

    } while (true);
}

void DisableWindowsFirewall()
{
    HRESULT hrComInit = S_OK;
    HRESULT hr = S_OK;

    INetFwPolicy2* pNetFwPolicy2 = nullptr;

    hrComInit = CoInitializeEx(
        0,
        COINIT_APARTMENTTHREADED
    );

    // Ignore RPC_E_CHANGED_MODE; this just means that COM has already been
    // initialized with a different mode. Since we don't care what the mode is,
    // we'll just use the existing mode.
    if (hrComInit != RPC_E_CHANGED_MODE)
    {
        if (FAILED(hrComInit))
        {
            printf("CoInitializeEx failed: 0x%08lx\n", hrComInit);
            goto Cleanup;
        }
    }

    hr = WFCOMInitialize(&pNetFwPolicy2);
    if (FAILED(hr))
    {
        goto Cleanup;
    }


    hr = pNetFwPolicy2->put_FirewallEnabled(NET_FW_PROFILE2_DOMAIN, false);
    if(FAILED(hr))
    {
        printf("put_FirewallEnabled failed for Domain: 0x%08lx\n", hr);
        goto Cleanup;
    }
    else
    {
        printf("Firewall disabled for Domain.\n");
    }

    hr = pNetFwPolicy2->put_FirewallEnabled(NET_FW_PROFILE2_PRIVATE, false);
    if (FAILED(hr))
    {
        printf("put_FirewallEnabled failed for Private: 0x%08lx\n", hr);
        goto Cleanup;
    }
    else
    {
        printf("Firewall disabled for Private.\n");
    }

    hr = pNetFwPolicy2->put_FirewallEnabled(NET_FW_PROFILE2_PUBLIC, false);
    if (FAILED(hr))
    {
        printf("put_FirewallEnabled failed for Public: 0x%08lx\n", hr);
        goto Cleanup;
    }
    else
    {
        printf("Firewall disabled for Public.\n");
    }


	Cleanup:
    WFCOMCleanup(pNetFwPolicy2);

    if(SUCCEEDED(hrComInit))
    {
        CoUninitialize();
    }
}

void ManageProfileState(NET_FW_PROFILE_TYPE2 profileType, bool enabled)
{
    HRESULT hrComInit = S_OK;
    HRESULT hr = S_OK;

    INetFwPolicy2* pNetFwPolicy2 = nullptr;

    hrComInit = CoInitializeEx(
        0,
        COINIT_APARTMENTTHREADED
    );

    // Ignore RPC_E_CHANGED_MODE; this just means that COM has already been
    // initialized with a different mode. Since we don't care what the mode is,
    // we'll just use the existing mode.
    if (hrComInit != RPC_E_CHANGED_MODE)
    {
        if (FAILED(hrComInit))
        {
            printf("CoInitializeEx failed: 0x%08lx\n", hrComInit);
            goto Cleanup;
        }
    }

    hr = WFCOMInitialize(&pNetFwPolicy2);
    if(FAILED(hr))
    {
        goto Cleanup;
    }


    if(profileType == NET_FW_PROFILE2_DOMAIN or profileType == NET_FW_PROFILE2_ALL)
    {
        hr = pNetFwPolicy2->put_FirewallEnabled(NET_FW_PROFILE2_DOMAIN, enabled);
        if (FAILED(hr))
        {
            printf("put_FirewallEnabled failed for Domain: 0x%08lx\n", hr);
            goto Cleanup;
        }
        if(enabled)
        {
            printf("Firewall enabled for Domain.\n");
        }
        else
        {
            printf("Firewall disabled for Domain.\n");
        }
    }

    if (profileType == NET_FW_PROFILE2_PRIVATE or profileType == NET_FW_PROFILE2_ALL)
    {
        hr = pNetFwPolicy2->put_FirewallEnabled(NET_FW_PROFILE2_PRIVATE, enabled);
        if (FAILED(hr))
        {
            printf("put_FirewallEnabled failed for Private: 0x%08lx\n", hr);
            goto Cleanup;
        }
        if (enabled)
        {
            printf("Firewall enabled for Private.\n");
        }
        else
        {
            printf("Firewall disabled for Private.\n");
        }
    }

    if (profileType == NET_FW_PROFILE2_PUBLIC or profileType == NET_FW_PROFILE2_ALL)
    {
        hr = pNetFwPolicy2->put_FirewallEnabled(NET_FW_PROFILE2_PUBLIC, enabled);
        if (FAILED(hr))
        {
            printf("put_FirewallEnabled failed for Public: 0x%08lx\n", hr);
            goto Cleanup;
        }
        if (enabled)
        {
            printf("Firewall enabled for Public.\n");
        }
        else
        {
            printf("Firewall disabled for Public.\n");
        }
    }

Cleanup:
    WFCOMCleanup(pNetFwPolicy2);

    if (SUCCEEDED(hrComInit))
    {
        CoUninitialize();
    }
}

void ManageProfileFwStateMenu(bool enabled)
{
    int menuItem = -1;
    string sMenuAction;

    if(enabled)
    {
        sMenuAction = "Choose profile to enable:\n";
    }
    else
    {
        sMenuAction = "Choose profile to disable:\n";
    }

    NET_FW_PROFILE_TYPE2 profileType{};

    while(menuItem < 1 or menuItem > 4)
    {
        cout << sMenuAction
    		 << "1. Domain\n"
    		 << "2. Private\n"
    		 << "3. Public\n";

    	if(enabled)
        {
            cout << "4. All\n";
        }

        cin >> menuItem;

        switch (menuItem)
        {
	        case 1: profileType = NET_FW_PROFILE2_DOMAIN; break;
	        case 2: profileType = NET_FW_PROFILE2_PRIVATE; break;
	        case 3: profileType = NET_FW_PROFILE2_PUBLIC; break;
	        case 4: profileType = NET_FW_PROFILE2_ALL; break;
	        default: cout << "Invalid menu item selected. Try again.\n"; break;
        }

    }
    ManageProfileState(profileType, enabled);
}

void GetFirewallSettings()
{
    HRESULT hrComInit = S_OK;
    HRESULT hr = S_OK;

    INetFwPolicy2* pNetFwPolicy2 = nullptr;

    hrComInit = CoInitializeEx(
        nullptr,
        COINIT_APARTMENTTHREADED);

    if(hrComInit != RPC_E_CHANGED_MODE)
    {
	    if(FAILED(hrComInit))
	    {
            printf("CoInitializeEx failed: 0x%08lx\n", hrComInit);
            goto Cleanup;
	    }
    }

    hr = WFCOMInitialize(&pNetFwPolicy2);
    if(FAILED(hr))
    {
        goto Cleanup;
    }

    printf("Settings for the firewall domain profile:\n");
    GetFirewallSettingsPerProfile(NET_FW_PROFILE2_DOMAIN, pNetFwPolicy2);

	printf("Settings for the firewall private profile:\n");
    GetFirewallSettingsPerProfile(NET_FW_PROFILE2_PRIVATE, pNetFwPolicy2);

	printf("Settings for the firewall public profile:\n");
    GetFirewallSettingsPerProfile(NET_FW_PROFILE2_PUBLIC, pNetFwPolicy2);

    UpdateSettingsPerProfile(pNetFwPolicy2);

Cleanup:
    WFCOMCleanup(pNetFwPolicy2);

    if (SUCCEEDED(hrComInit))
    {
        CoUninitialize();
    }
}
void GetFirewallSettingsPerProfile(NET_FW_PROFILE_TYPE2 profileType, INetFwPolicy2* pNetFwPolicy2)
{
    VARIANT_BOOL bIsEnabled = FALSE;
    NET_FW_ACTION action;

	wprintf(L"---------------------------------------------\n");

    if (SUCCEEDED(pNetFwPolicy2->get_FirewallEnabled(profileType, &bIsEnabled)))
    {
        printf("Firewall is %s\n", bIsEnabled ? "on" : "off");
    }

    if (SUCCEEDED(pNetFwPolicy2->get_BlockAllInboundTraffic(profileType, &bIsEnabled)))
    {
        printf("Inbound connections that do not match a rule are %s\n", bIsEnabled ? "allowed" : "blocked");
    }

    if (SUCCEEDED(pNetFwPolicy2->get_NotificationsDisabled(profileType, &bIsEnabled)))
    {
        printf("Display a notification: %s\n", bIsEnabled ? "no" : "yes");
    }

    if (SUCCEEDED(pNetFwPolicy2->get_UnicastResponsesToMulticastBroadcastDisabled(profileType, &bIsEnabled)))
    {
        printf("Allow unicast response: %s\n", bIsEnabled ? "no" : "yes");
    }

    if (SUCCEEDED(pNetFwPolicy2->get_DefaultInboundAction(profileType, &action)))
    {
        printf("Default inbound action is %s\n", action != NET_FW_ACTION_BLOCK ? "Allow" : "Block");
    }

    if (SUCCEEDED(pNetFwPolicy2->get_DefaultOutboundAction(profileType, &action)))
    {
        printf("Default outbound action is %s\n", action != NET_FW_ACTION_BLOCK ? "Allow" : "Block");
    }

	printf("\n");
}

void UpdateSettingsPerProfile(INetFwPolicy2* pNetFwPolicy2) {
	CHAR cInput;

	cout << "Options: Change settings per profile (y/n)\n";
    cin >> cInput;

	if (cInput == 'n') return;

	const NET_FW_PROFILE_TYPE2 profileType = getProfileType();

	const VARIANT_BOOL bIsFwEnabled = getYesNoInput("Turn Firewall on");
	const VARIANT_BOOL bIsInboundEnabled = !getYesNoInput("Block inbound connections");
	const VARIANT_BOOL bAreNotificationsDisabled = getYesNoInput("Disable notifications");
	const VARIANT_BOOL bIsUnicastResponseDisabled = getYesNoInput("Disable unicast to multicast broadcast");

	const NET_FW_ACTION defaultInboundAction = getYesNoInput("Default block inbound traffic") ? NET_FW_ACTION_BLOCK : NET_FW_ACTION_ALLOW;
	const NET_FW_ACTION defaultOutboundAction = getYesNoInput("Default allow outbound traffic") ? NET_FW_ACTION_ALLOW : NET_FW_ACTION_BLOCK;

	const FwProfileManager fwProfileManager(profileType, defaultInboundAction, defaultOutboundAction, bIsFwEnabled, bIsInboundEnabled, bAreNotificationsDisabled, bIsUnicastResponseDisabled);
	UpdateProfile(fwProfileManager, pNetFwPolicy2);
}

void UpdateProfile(FwProfileManager fwProfileManager, INetFwPolicy2* pNetFwPolicy2)
{
    HRESULT hr = S_OK;
	const NET_FW_PROFILE_TYPE2 profileType = fwProfileManager.get_ProfileType();

    hr = pNetFwPolicy2->put_FirewallEnabled(profileType, fwProfileManager.get_IsFwEnabled());
    if(FAILED(hr))
    {
        printf("put_FirewallEnabled failed: 0x%08lx\n", hr);
        return;
    }

    hr = pNetFwPolicy2->put_BlockAllInboundTraffic(profileType, fwProfileManager.get_IsInboundEnabled());
    if (FAILED(hr))
    {
        printf("put_BlockAllInboundTraffic failed: 0x%08lx\n", hr);
        return;
    }

    hr = pNetFwPolicy2->put_NotificationsDisabled(profileType, fwProfileManager.get_AreNotificationsDisabled());
    if (FAILED(hr))
    {
    	printf("put_NotificationsDisabled failed: 0x%08lx\n", hr);
    	return;
    }

    hr = pNetFwPolicy2->put_UnicastResponsesToMulticastBroadcastDisabled(profileType, fwProfileManager.get_IsUnicastResponseDIsabled());
    if (FAILED(hr))
    {
        printf("put_UnicastResponsesToMulticastBroadcastDisabled failed: 0x%08lx\n", hr);
        return;
    }

    hr = pNetFwPolicy2->put_DefaultInboundAction(profileType, fwProfileManager.get_DefaultInboundAction());
    if (FAILED(hr))
    {
        printf("put_DefaultInboundAction failed: 0x%08lx\n", hr);
        return;
    }

    hr = pNetFwPolicy2->put_DefaultOutboundAction(profileType, fwProfileManager.get_DefaultOutboundAction());
    if (FAILED(hr))
    {
        printf("put_DefaultOutboundAction failed: 0x%08lx\n", hr);
        return;
    }

    printf("Successfully updated profile settings\n");
}