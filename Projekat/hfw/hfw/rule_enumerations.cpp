/********************************************************************++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) Microsoft Corporation. All Rights Reserved.

Abstract:
    This C++ file includes sample code for enumerating Windows Firewall
    rules using the Microsoft Windows Firewall APIs.

********************************************************************/

#include "rule_enumerations.h"
#include "string_helper.h"
#include "rule_settings.h"
#include "enums.h"
#include <windows.h>
#include <winsock.h> 
#include <atlcomcli.h>
#include <iostream>
#include <shlwapi.h>
#include <vector>

using namespace std;

int SelectFwRuleIndex(int maxSize)
{
    int index;
	cout << "Enter index of the rule: ";
    do
    {
        cin >> index;
        if (index < 0 or index > maxSize - 1)
        {
            cout << "Invalid index selected. Try again.\n";
        }
    } while (index < 0 or index > maxSize - 1);
    return index;
}

bool ChangeFwRuleEnabled(std::vector<CComPtr<INetFwRule>>& fwRulesVec, int position)
{
    INetFwRule* fwRule = fwRulesVec.at(position);
	VARIANT_BOOL enabled;
    if(FAILED(fwRule->get_Enabled(&enabled)))
    {
        wprintf(L"get_Enabled failed. Cannot change the enabled property of the rule\n");
        return false;
    }
    if (FAILED(fwRule->put_Enabled(!enabled)))
    {
        wprintf(L"put_Enabled failed. Cannot change the enabled property of the rule\n");
        return false;
    }
    wprintf(L"Enabled property of the rule changed.\n");
    fwRulesVec.at(position) = fwRule;
    return true;
}

bool RemoveFwRule(INetFwRules* fwRules, std::vector<CComPtr<INetFwRule>>& fwRulesVec, int position)
{
    HRESULT hr = S_OK;
    CComBSTR bstrOldName;
    CComBSTR bstrTempName = GenerateGUIDString();
    if(bstrTempName == nullptr)
    {
        wprintf(L"CoCreateGuid failed\n");
        return false;
    }
	const CComPtr<INetFwRule> fwRule = fwRulesVec.at(position);

    hr = fwRule->get_Name(&bstrOldName);
    if (FAILED(hr))
    {
        wprintf(L"get_Name failed. Cannot remove firewall rule: %lx\n", hr);
        return false;
    }
    hr = fwRule->put_Name(bstrTempName);
    if(FAILED(hr))
    {
        wprintf(L"put_Name failed. Cannot remove firewall rule: %lx\n", hr);
        return false;
    }
    hr = fwRules->Remove(bstrTempName);
    if(FAILED(hr))
    {
        wprintf(L"Remove failed: %lx\n", hr);
        hr = fwRule->put_Name(bstrOldName); //Reset to old name
    	return false;
    }
    fwRulesVec.erase(fwRulesVec.begin() + position);
    wprintf(L"Successfully removed rule %ls\n", static_cast<BSTR>(bstrOldName));
    return true;
}

void PrintLocalizableString(PCWSTR label, PCWSTR value)
{
    wchar_t buffer[256];
    if (value[0] == L'@' &&
        SUCCEEDED(SHLoadIndirectString(value, buffer, ARRAYSIZE(buffer), nullptr)))
    {
        value = buffer;
    }
    wprintf(L"%s%s\n", label, value);
}

void DumpFWRulesInCollection(INetFwRule* fwRule, int position)
{
    wprintf(L"---------------------------------------------\n");
    wprintf(L"Index:            %ld\n", position);

    CComBSTR name;
    if(SUCCEEDED(fwRule->get_Name(&name)) && name)
    {
        PrintLocalizableString(L"Name:             ", name);
    }

    CComBSTR description;
    if(SUCCEEDED(fwRule->get_Description(&description)) && description)
    {
        PrintLocalizableString(L"Description:      ", description);
    }

    CComBSTR applicationName;
    if(SUCCEEDED(fwRule->get_ApplicationName(&applicationName)) && applicationName)
    {
        wprintf(L"Application Name: %ls\n", static_cast<BSTR>(applicationName));
    }

    CComBSTR serviceName;
    if (SUCCEEDED(fwRule->get_ServiceName(&serviceName)) && serviceName)
    {
        wprintf(L"Service Name:     %ls\n", static_cast<BSTR>(serviceName));
    }

    long protocolNumber;
    if(SUCCEEDED(fwRule->get_Protocol(&protocolNumber)))
    {
        PCSTR protocolName = nullptr;
        if(protocolNumber == NET_FW_IP_PROTOCOL_ANY)
        {
            protocolName = "Any";
        }
        else
        {
            protoent* ent = getprotobynumber(protocolNumber);
            if (ent)
            {
                protocolName = ent->p_name;
            }
	        
        }
        if (protocolName)
        {
            wprintf(L"IP Protocol:      %d (%hs)\n", protocolNumber, protocolName);
        }
        else
        {
            wprintf(L"IP Protocol:      %d\n", protocolNumber);
        }

        if (protocolNumber != IP_PROTOCOL_ICMP4 && protocolNumber != IP_PROTOCOL_ICMP6)
        {
            CComBSTR localPorts;
            if (SUCCEEDED(fwRule->get_LocalPorts(&localPorts)) && localPorts)
            {
                wprintf(L"Local Ports:      %ls\n", static_cast<BSTR>(localPorts));
            }

            CComBSTR remotePorts;
            if (SUCCEEDED(fwRule->get_RemotePorts(&remotePorts)) && remotePorts)
            {
                wprintf(L"Remote Ports:     %ls\n", static_cast<BSTR>(remotePorts));
            }
        }
        else
        {
            CComBSTR icmpTypesAndCodes;
            if (SUCCEEDED(fwRule->get_IcmpTypesAndCodes(&icmpTypesAndCodes)) && icmpTypesAndCodes)
            {
                wprintf(L"ICMP TypeCode:    %ls\n", static_cast<BSTR>(icmpTypesAndCodes));
            }
        }
    }

    CComBSTR localAddresses;
    if (SUCCEEDED(fwRule->get_LocalAddresses(&localAddresses)) && localAddresses)
    {
        wprintf(L"LocalAddresses:   %ls\n", static_cast<BSTR>(localAddresses));
    }

    CComBSTR remoteAddresses;
    if (SUCCEEDED(fwRule->get_RemoteAddresses(&remoteAddresses)) && remoteAddresses)
    {
        wprintf(L"RemoteAddresses:  %ls\n", static_cast<BSTR>(remoteAddresses));
    }

    long lProfileBitmask = 0;
    if (SUCCEEDED(fwRule->get_Profiles(&lProfileBitmask)))
    {
        // The returned bitmask can have more than 1 bit set if multiple profiles 
        // are active or current at the same time
        static const struct ProfileMapElement
        {
            NET_FW_PROFILE_TYPE2 Id;
            LPCWSTR Name;
        } ProfileMap[3] = {
            { NET_FW_PROFILE2_DOMAIN, L"Domain" },
            { NET_FW_PROFILE2_PRIVATE, L"Private" },
            { NET_FW_PROFILE2_PUBLIC, L"Public" },
        };

        bool first = true;
        wprintf(L"Profiles:         ");

        for (ProfileMapElement const& entry : ProfileMap)
        {
            if (lProfileBitmask & entry.Id)
            {
                if (!first)
                {
                    wprintf(L", ");
                }
                first = false;
                wprintf(L"%ls", entry.Name);
            }
        }
        wprintf(L"\n");
    }

    NET_FW_RULE_DIRECTION fwDirection;
    if (SUCCEEDED(fwRule->get_Direction(&fwDirection)))
    {
        switch (fwDirection)
        {
        case NET_FW_RULE_DIR_IN:
            wprintf(L"Direction:        In\n");
            break;

        case NET_FW_RULE_DIR_OUT:
            wprintf(L"Direction:        Out\n");
            break;
        }
    }

    NET_FW_ACTION fwAction;
    if (SUCCEEDED(fwRule->get_Action(&fwAction)))
    {
        switch (fwAction)
        {
        case NET_FW_ACTION_BLOCK:
            wprintf(L"Action:           Block\n");
            break;

        case NET_FW_ACTION_ALLOW:
            wprintf(L"Action:           Allow\n");
            break;
        }
    }

    CComVariant InterfaceArray;
    if (SUCCEEDED(fwRule->get_Interfaces(&InterfaceArray)))
    {
        if (InterfaceArray.vt == (VT_VARIANT | VT_ARRAY))
        {
            SAFEARRAY* pSa = NULL;

            pSa = InterfaceArray.parray;

            for (long index = pSa->rgsabound->lLbound; index < static_cast<long>(pSa->rgsabound->cElements); index++)
            {
                CComVariant InterfaceString;
                if (SUCCEEDED(SafeArrayGetElement(pSa, &index, &InterfaceString)) && InterfaceString.vt == VT_BSTR)
                {
                    wprintf(L"Interfaces:       %ls\n", InterfaceString.bstrVal);
                }
            }
        }
    }

    CComBSTR interfaceTypes;
    if (SUCCEEDED(fwRule->get_InterfaceTypes(&interfaceTypes)) && interfaceTypes)
    {
        wprintf(L"Interface Types:  %ls\n", static_cast<BSTR>(interfaceTypes));
    }

    VARIANT_BOOL enabled;
    if (SUCCEEDED(fwRule->get_Enabled(&enabled)))
    {
        wprintf(L"Enabled:          %ls\n", enabled ? L"TRUE" : L"FALSE");
    }

    CComBSTR grouping;
    if (SUCCEEDED(fwRule->get_Grouping(&grouping)) && grouping)
    {
        PrintLocalizableString(L"Grouping:         ", grouping);
    }

    if (SUCCEEDED(fwRule->get_EdgeTraversal(&enabled)))
    {
        wprintf(L"Edge Traversal:   %ls\n", enabled ? L"TRUE" : L"FALSE");
    }
}

void EnumerateFwRuleByDirection(const CComPtr<IEnumVARIANT>& pVariant, vector<CComPtr<INetFwRule>>& fwNetRules, NET_FW_RULE_DIRECTION fwDirection)
{
	HRESULT hr = S_OK;
	ULONG cFetched = 0;
	if(!fwNetRules.empty())
	{
		for (int i=0; i < fwNetRules.size(); i++)
		{
			DumpFWRulesInCollection(fwNetRules.at(i), i);
		}
	}
	else
	{
		for (CComVariant var; pVariant->Next(1, &var, &cFetched) == S_OK; var.Clear())
		{
			CComPtr<INetFwRule> pFwRule;
			if (SUCCEEDED(var.ChangeType(VT_DISPATCH)) &&
				SUCCEEDED(V_DISPATCH(&var)->QueryInterface(IID_PPV_ARGS(&pFwRule))))
			{
				NET_FW_RULE_DIRECTION dir;
				hr = pFwRule->get_Direction(&dir);
				if (SUCCEEDED(hr) && dir != fwDirection)
				{
					continue;
				}
				fwNetRules.push_back(pFwRule);
				DumpFWRulesInCollection(pFwRule,  static_cast<int>(fwNetRules.size()) - 1);
			}
		}
	}
}

void EnumerateFwRulesMenu(NET_FW_RULE_DIRECTION fwDirection)
{
    CComPtr<INetFwPolicy2> pNetFwPolicy2;
    HRESULT hr = pNetFwPolicy2.CoCreateInstance(_uuidof(NetFwPolicy2));
    if(FAILED(hr))
    {
        wprintf(L"CoCreateInstance failed: 0x%08lx\n", hr);
        return;
    }

    CComPtr<INetFwRules> pFwRules;
    hr = pNetFwPolicy2->get_Rules(&pFwRules);
    if(FAILED(hr))
    {
        wprintf(L"get_Rules failed: 0x%08lx\n", hr);
        return;
    }

    long fwRuleCount;
    hr = pFwRules->get_Count(&fwRuleCount);
    if (FAILED(hr))
    {
        wprintf(L"get_Count failed: 0x%08lx\n", hr);
        return;
    }
    wprintf(L"The number of rules in the Windows Firewall are %d\n", fwRuleCount);

    CComPtr<IUnknown> pEnumerator;
    hr = pFwRules->get__NewEnum(&pEnumerator);
    if(FAILED(hr))
    {
        wprintf(L"get__NewEnum failed: 0x%08lx\n", hr);
        return;
    }

    CComPtr<IEnumVARIANT> pVariant;
    hr = pEnumerator.QueryInterface(&pVariant);
    if (FAILED(hr))
    {
        wprintf(L"get__NewEnum failed to produce IEnumVariant: 0x%08lx\n", hr);
        return;
    }

    vector<CComPtr<INetFwRule>> fwNetRules;
	EnumerateFwRuleByDirection(pVariant, fwNetRules, fwDirection);

    int menuItem;
    int index;
    int fwNetRulesSize = static_cast<int>(fwNetRules.size());
	do
    {
        cout << "\nActions:\n"
            << "1. Enable\\Disable a rule\n"
            << "2. Delete rule\n"
            << "3. Update rule\n"
            << "0. Exit\n";

        cin.clear();
        cin >> menuItem;

        switch (menuItem)
        {
			case 0: return;
            case 1:
                {
                    index = SelectFwRuleIndex(fwNetRulesSize);
                    if (ChangeFwRuleEnabled(fwNetRules, index)) 
                    {
                        EnumerateFwRuleByDirection(pVariant, fwNetRules, fwDirection);
                    }
                    break;
				}
			case 2:
				{
					index = SelectFwRuleIndex(fwNetRulesSize);
                    if (RemoveFwRule(pFwRules, fwNetRules, index)) 
                    {
                        EnumerateFwRuleByDirection(pVariant, fwNetRules, fwDirection);
                    }
					break;
				}
            case 3:
                {
                    index = SelectFwRuleIndex(fwNetRulesSize);
                    CComPtr<INetFwRule> toUpdate = fwNetRules.at(index);
                    UpdateFwRule(toUpdate, index);
                    return;
                }
	        default: cout << "Invalid menu item selected. Try again.\n"; break;
        }
    }
    while (menuItem != 0);
}

void EnumerateFWRules(NET_FW_RULE_DIRECTION fwDirection)
{
    if(SUCCEEDED(CoInitialize(nullptr)))
    {
        WSADATA wsaData;
        int err = WSAStartup(MAKEWORD(2, 2), &wsaData);

        EnumerateFwRulesMenu(fwDirection);

        if(err == 0)
        {
            WSACleanup();
        }
        CoUninitialize();
    }
}