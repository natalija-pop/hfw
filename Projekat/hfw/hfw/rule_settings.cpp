#include "rule_settings.h"
#include <iostream>
#include <netfw.h>
#include <string>

#include "helper.h"
#include "enums.h"

using namespace std;

bool IsEmpty(const BSTR& field)
{
	return SysStringLen(field) < 1;
}

void CreateNewRule(const FwRule& fwNewRule)
{
	HRESULT hrComInit = S_OK;
	HRESULT hr = S_OK;

	INetFwPolicy2* pNetFwPolicy2 = nullptr;
	INetFwRules* pNetFwRules = nullptr;
	INetFwRule* pNetFwRule = nullptr;

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

	hr = pNetFwPolicy2->get_Rules(&pNetFwRules);
	if(FAILED(hr))
	{
		printf("get_Rules failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

	hr = CoCreateInstance(
		__uuidof(NetFwRule),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(INetFwRule),
		(void**)&pNetFwRule);
	if (FAILED(hr))
	{
		printf("CoCreateInstance for Firewall Rule failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

	hr = pNetFwRule->put_Name(fwNewRule.getName());
	if(FAILED(hr))
	{
		printf("put_Name failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

	if(!IsEmpty(fwNewRule.getDescription()))
	{
		hr = pNetFwRule->put_Description(fwNewRule.getDescription());
		if(FAILED(hr))
		{
			printf("put_Description failed: 0x%08lx\n", hr);
			goto Cleanup;
		}
	}

	if(!IsEmpty(fwNewRule.getApplicationName()))
	{
		hr = pNetFwRule->put_ApplicationName(fwNewRule.getApplicationName());
		if (FAILED(hr))
		{
			printf("put_ApplicationName failed: 0x%08lx\n", hr);
			goto Cleanup;
		}
	}

	hr = pNetFwRule->put_Profiles(fwNewRule.getProfilesBitMask());
	if (FAILED(hr))
	{
		printf("put_Profiles failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

	hr = pNetFwRule->put_Action(fwNewRule.getAction());
	if(FAILED(hr))
	{
		printf("put_Action failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

	hr = pNetFwRule->put_Direction(fwNewRule.getDirection());
	if(FAILED(hr))
	{
		printf("put_Direction failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

	hr = fwNewRule.getProtocol() != -1 ? pNetFwRule->put_Protocol(fwNewRule.getProtocol()) : pNetFwRule->put_Protocol(NET_FW_IP_PROTOCOL_ANY);
	if (FAILED(hr))
	{
		printf("put_Protocol failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

	if ((fwNewRule.getProtocol() == IP_PROTOCOL_ICMP4 or fwNewRule.getProtocol() == IP_PROTOCOL_ICMP6) and 
		!IsEmpty(fwNewRule.getICMPTypeCode()))
	{
		hr = pNetFwRule->put_IcmpTypesAndCodes(fwNewRule.getICMPTypeCode());
		if (FAILED(hr))
		{
			printf("put_IcmpTypesAndCodes failed: 0x%08lx\n", hr);
			goto Cleanup;
		}
	}

	if(fwNewRule.getProtocol() == IP_PROTOCOL_TCP or fwNewRule.getProtocol() == IP_PROTOCOL_UDP)
	{
		if(!IsEmpty(fwNewRule.getLocalPorts()))
		{
			hr = pNetFwRule->put_LocalPorts(fwNewRule.getLocalPorts());
			if (FAILED(hr))
			{
				printf("put_LocalPorts failed: 0x%08lx\n", hr);
				goto Cleanup;
			}
		}

		if(!IsEmpty(fwNewRule.getRemotePorts()))
		{
			hr = pNetFwRule->put_RemotePorts(fwNewRule.getRemotePorts());
			if (FAILED(hr))
			{
				printf("put_RemotePorts failed: 0x%08lx\n", hr);
				goto Cleanup;
			}
		}
	}

	hr = pNetFwRule->put_Enabled(VARIANT_TRUE);
	if (FAILED(hr))
	{
		printf("put_Enabled failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

	hr = pNetFwRules->Add(pNetFwRule);
	if (FAILED(hr))
	{
		printf("Firewall Rule Add failed: 0x%08lx\n", hr);
	}

Cleanup:

	if (pNetFwRule != nullptr)
	{
		pNetFwRule->Release();
	}

	if (pNetFwRules != nullptr)
	{
		pNetFwRules->Release();
	}

	WFCOMCleanup(pNetFwPolicy2);

	if (SUCCEEDED(hrComInit))
	{
		CoUninitialize();
	}
}

void AddNewRule()
{
	FwRule newFwRule = FwRule();

	BSTR bstrName = nullptr;
	BSTR bstrDescription = nullptr;
	BSTR bstrApplicationName = nullptr;
	BSTR bstrLocalPorts = nullptr;
	BSTR bstrRemotePorts = nullptr;
	BSTR bstrICMPTypeCode = nullptr; // Only for ICMP rule

	LONG lProtocol = -1;
	LONG lProfilesBitMask = 0;

	NET_FW_RULE_DIRECTION direction;
	NET_FW_ACTION action;

	bstrApplicationName = EnterFwRuleAppName();
	newFwRule.setApplicationName(bstrApplicationName);

	lProtocol = EnterFwRuleProtocolNumber();
	newFwRule.setProtocol(lProtocol);

	if(lProtocol == IP_PROTOCOL_TCP or lProtocol == IP_PROTOCOL_UDP)
	{
		bstrLocalPorts =  EnterFwRulePorts(true, lProtocol == IP_PROTOCOL_TCP);
		newFwRule.setLocalPorts(bstrLocalPorts);

		bstrRemotePorts =  EnterFwRulePorts(false, lProtocol == IP_PROTOCOL_TCP);
		newFwRule.setRemotePorts(bstrRemotePorts);
	}

	else if(lProtocol == IP_PROTOCOL_ICMP4 or lProtocol == IP_PROTOCOL_ICMP6)
	{
		bstrICMPTypeCode = EnterFwRuleICMPTypes(lProtocol == 1);
		newFwRule.setICMPTypeCode(bstrICMPTypeCode);
	}

	direction = EnterFwRuleDirection();
	newFwRule.setDirection(direction);

	action = EnterFwRuleAction();
	newFwRule.setAction(action);

	lProfilesBitMask = EnterFwRuleProfiles();
	newFwRule.setProfilesBitMask(lProfilesBitMask);

	bstrName = EnterFwRuleName();
	newFwRule.setName(bstrName);

	bstrDescription = EnterFwRuleDescription();
	newFwRule.setDescription(bstrDescription);

	CreateNewRule(newFwRule);

//Cleanup:

	if (bstrName != nullptr) 
	{
		SysFreeString(bstrName);
	}
	if (bstrDescription != nullptr) 
	{
		SysFreeString(bstrDescription);
	}
	if (bstrApplicationName != nullptr) 
	{
		SysFreeString(bstrApplicationName);
	}
	if (bstrLocalPorts != nullptr) 
	{
		SysFreeString(bstrLocalPorts);
	}
	if (bstrRemotePorts != nullptr) 
	{
		SysFreeString(bstrRemotePorts);
	}
	if (bstrICMPTypeCode != nullptr) 
	{
		SysFreeString(bstrICMPTypeCode);
	}
}