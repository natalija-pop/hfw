/********************************************************************++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) Microsoft Corporation. All Rights Reserved.

--********************************************************************/

#include "helper.h"
#include "string_helper.h"
#include "port_handler.h"
#include "icmp_types_handler.h"
#include <atlcomcli.h>
#include <netfw.h>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

HRESULT WFCOMInitialize(INetFwPolicy2** ppNetFwPolicy2)
{
    HRESULT hr = S_OK;

    hr = CoCreateInstance(
        __uuidof(NetFwPolicy2),
        NULL,
        CLSCTX_INPROC_SERVER,
        __uuidof(INetFwPolicy2),
        (void**)ppNetFwPolicy2);

    if (FAILED(hr))
    {
        printf("CoCreateInstance for INetFwPolicy2 failed: 0x%08lx\n", hr);
        goto Cleanup;
    }

Cleanup:
    return hr;
}

void WFCOMCleanup(INetFwPolicy2* pNetFwPolicy2)
{
    if (pNetFwPolicy2 != nullptr)
    {
        pNetFwPolicy2->Release();
    }
}

void EnterInterface(const std::string interfaceType, std::string& sInterfaces)
{
    cout << interfaceType << " (y/n): ";
    if (EnterYesNoInput() == 'y')
    {
        if (!sInterfaces.empty())
        {
            sInterfaces += ",";
        }
        sInterfaces += interfaceType;
    }
}

LONG EnterFwRuleProtocolNumber()
{
    LONG lProtocolNumber;

    cout << "PROTOCOL\n";
    cout << "This is the list of predefined protocols to choose from:\n";

    for(const auto& protocol: protocol_numbers::protocols)
    {
        cout << protocol << "\n";
    }

	cout << "Enter the number of the protocol from the list or enter custom number for custom protocol (0-255): ";
    do
    {
        cin >> lProtocolNumber;

        if (lProtocolNumber < 0 or lProtocolNumber > 256)
        {
            cout << "Invalid enter for protocol number. Try again.\n";
        }
    }
    while (lProtocolNumber < 0 or lProtocolNumber > 256);
    return lProtocolNumber;
}

LONG EnterFwRuleProfiles()
{
	long fwRuleProfilesBitMask = 0;
    cout << "PROFILES\n";
    cout << "When does this rule apply?\n";

	cout << "DOMAIN\nApplies when computer is connected to its corporate domain (y/n): ";
    char input = EnterYesNoInput();
	if(input == 'y')
    {
        fwRuleProfilesBitMask |= NET_FW_PROFILE2_DOMAIN;
    }

    cout << "PRIVATE\nApplies when computer is connected to a private network location, \nsuch as home or work place (y/n): ";
    input = EnterYesNoInput();
    if (input == 'y')
    {
        fwRuleProfilesBitMask |= NET_FW_PROFILE2_PRIVATE;
    }
	cout << "PUBLIC\nApplies when computer is connected to a public network location (y/n): ";
    input = EnterYesNoInput();
    if (input == 'y')
    {
        fwRuleProfilesBitMask |= NET_FW_PROFILE2_PUBLIC;
    }
    return fwRuleProfilesBitMask;
}

NET_FW_RULE_DIRECTION EnterFwRuleDirection()
{
    LONG direction;
    cout << "DIRECTION\n";
    cout << "This is the list of allowed directions:\n"
	<< "1. Inbound\n"
	<< "2. Outbound\n"
    << "Enter the number of the direction from the list: ";
    do
    {
        cin >> direction;
        if (direction < 1 or direction >= NET_FW_RULE_DIR_MAX)
        {
            cout << "Invalid enter for direction. Try again.\n";
        }
    }
    while (direction < 1 or direction >= NET_FW_RULE_DIR_MAX);
    return direction == 1 ? NET_FW_RULE_DIR_IN : NET_FW_RULE_DIR_OUT;
}

NET_FW_ACTION EnterFwRuleAction()
{
    LONG action;
    cout << "ACTION\n";
    cout << "This is the list of allowed actions:\n"
        << "1. Block the connection\n"
        << "2. Allow the connection\n"
        << "Enter the number of the action from the list: ";
    do
    {
        cin >> action;
        if (action < 1 or action > NET_FW_ACTION_MAX)
        {
            cout << "Invalid enter for action. Try again.\n";
        }
    } while (action < 1 or action > NET_FW_ACTION_MAX);
    return action == 1 ? NET_FW_ACTION_BLOCK : NET_FW_ACTION_ALLOW;
}

BSTR EnterFwRuleAppName()
{
    string sApplicationName;
    cout << "APPLICATION NAME\n";
	cout << "Application name (Leave empty for all applications): ";

    cin.clear();
    cin.ignore(INT_MAX, '\n');
	getline(std::cin, sApplicationName);
    return ConvertStringToBSTR(sApplicationName);
}

BSTR EnterFwRuleName()
{
    string sName;
    cout << "NAME\n";
    cout << "Enter the name for the rule: ";
    do
    {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        getline(cin, sName);
        if (sName.find_first_of('|') != string::npos or sName == "all")
        {
            cout << "Invalid enter for name. Try again.\n";
        }
    }
    while (sName.find_first_of('|') != string::npos or sName == "all");
    return ConvertStringToBSTR(sName);
}

BSTR EnterFwRuleDescription()
{
    string sDescription;
    cout << "Description (optional): ";
    do
    {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        getline(cin, sDescription);
        if (sDescription.find_first_of('|') != string::npos)
        {
            cout << "Description must not contain delimiter '|'. Try again.\n";
        }
    }
    while (sDescription.find_first_of('|') != string::npos);
    return ConvertStringToBSTR(sDescription);
}

BSTR EnterFwRulePorts(bool isLocal, bool isProtocolTCP)
{
    return EnterPorts(isLocal, isProtocolTCP);
}

BSTR EnterFwRuleICMPTypes(bool isVersion4)
{
    return EnterICMPTypes(isVersion4);
}

BSTR EnterFwRuleInterfaceTypes()
{
    string sInterfaces;
    cout << "INTERFACES\n";
    cout << "Choose interface types this rule applies to:\nAcceptable values are \"RemoteAccess\", \"Wireless\", \"Lan\", and \"All\"\n";
    cout << "All (y/n):";
    if (EnterYesNoInput() == 'y') 
    {
        sInterfaces = "All";
        goto ConvertToBSTR;
    }

    EnterInterface("RemoteAccess", sInterfaces);
    
    EnterInterface("Wireless", sInterfaces);
    
    EnterInterface("Lan", sInterfaces);
    
ConvertToBSTR:
    return ConvertStringToBSTR(sInterfaces);
}