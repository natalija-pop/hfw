#include "rule_settings.h"
#include <iostream>
#include <netfw.h>
#include <string>

#include "helper.h"
#include "string_helper.h"
#include "enums.h"
#include "rule_enumerations.h"

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

void UpdateFwRuleChanges(CComPtr<INetFwRule> fwRule, const FwRule& updatedFwRule)
{
	HRESULT hr = S_OK;

	CComBSTR bstrProperty;
	long lProperty = 0;
	VARIANT_BOOL enabled;
	NET_FW_RULE_DIRECTION direction;
	NET_FW_ACTION action;

	hr = fwRule->get_Name(&bstrProperty);
	if (SUCCEEDED(hr) && bstrProperty != updatedFwRule.getName())
	{
		hr = fwRule->put_Name(updatedFwRule.getName());
		if (FAILED(hr))
		{
			wprintf(L"put_Name failed: 0x%08lx\n", hr);
		}
	}
	bstrProperty.Empty(); // Clear the BSTR

	hr = fwRule->get_Description(&bstrProperty);
	if (SUCCEEDED(hr) && bstrProperty != updatedFwRule.getDescription())
	{
		hr = fwRule->put_Description(updatedFwRule.getDescription());
		if (FAILED(hr))
		{
			wprintf(L"put_Description failed: 0x%08lx\n", hr);
		}
	}
	bstrProperty.Empty(); // Clear the BSTR

	hr = fwRule->get_ApplicationName(&bstrProperty);
	if (SUCCEEDED(hr) && bstrProperty != updatedFwRule.getApplicationName())
	{
		hr = fwRule->put_ApplicationName(updatedFwRule.getApplicationName());
		if (FAILED(hr))
		{
			wprintf(L"put_ApplicationName failed: 0x%08lx\n", hr);
		}
	}
	bstrProperty.Empty(); // Clear the BSTR

	hr = fwRule->get_Profiles(&lProperty);
	if (SUCCEEDED(hr) && lProperty != updatedFwRule.getProfilesBitMask())
	{
		hr = fwRule->put_Profiles(updatedFwRule.getProfilesBitMask());
		if (FAILED(hr))
		{
			wprintf(L"put_Profiles failed: 0x%08lx\n", hr);
		}
	}

	hr = fwRule->get_Direction(&direction);
	if (SUCCEEDED(hr) && direction != updatedFwRule.getDirection())
	{
		hr = fwRule->put_Direction(updatedFwRule.getDirection());
		if (FAILED(hr))
		{
			wprintf(L"put_Direction failed: 0x%08lx\n", hr);
		}
	}

	hr = fwRule->get_Action(&action);
	if (SUCCEEDED(hr) && action != updatedFwRule.getAction())
	{
		hr = fwRule->put_Action(updatedFwRule.getAction());
		if (FAILED(hr))
		{
			wprintf(L"put_Action failed: 0x%08lx\n", hr);
		}
	}

	hr = fwRule->get_Protocol(&lProperty);
	if (SUCCEEDED(hr))
	{
		if (lProperty != updatedFwRule.getProtocol())
		{
			if (updatedFwRule.getProtocol() == IP_PROTOCOL_TCP or updatedFwRule.getProtocol() == IP_PROTOCOL_UDP)
			{
				hr = fwRule->put_IcmpTypesAndCodes(nullptr);
				if (FAILED(hr))
				{
					wprintf(L"put_IcmpTypesAndCodes failed: 0x%08lx\n", hr);
				}

				hr = fwRule->put_Protocol(updatedFwRule.getProtocol());
				if (FAILED(hr))
				{
					wprintf(L"put_Protocol failed: 0x%08lx\n", hr);
				}

				hr = fwRule->get_LocalPorts(&bstrProperty);
				if (SUCCEEDED(hr) && bstrProperty != updatedFwRule.getLocalPorts())
				{
					hr = fwRule->put_LocalPorts(updatedFwRule.getLocalPorts());
					if (FAILED(hr))
					{
						wprintf(L"put_LocalPorts failed: 0x%08lx\n", hr);
					}
				}
				bstrProperty.Empty(); // Clear the BSTR

				hr = fwRule->get_RemotePorts(&bstrProperty);
				if (SUCCEEDED(hr) && bstrProperty != updatedFwRule.getRemotePorts())
				{
					hr = fwRule->put_RemotePorts(updatedFwRule.getRemotePorts());
					if (FAILED(hr))
					{
						wprintf(L"put_RemotePorts failed: 0x%08lx\n", hr);
					}
				}
				bstrProperty.Empty(); // Clear the BSTR
			}
			else if (updatedFwRule.getProtocol() == IP_PROTOCOL_ICMP4 or updatedFwRule.getProtocol() == IP_PROTOCOL_ICMP6)
			{
				hr = fwRule->put_LocalPorts(nullptr);
				hr = fwRule->put_RemotePorts(nullptr);
				hr = fwRule->put_Protocol(updatedFwRule.getProtocol());
				if (FAILED(hr))
				{
					wprintf(L"put_Protocol failed: 0x%08lx\n", hr);
				}
				hr = fwRule->get_IcmpTypesAndCodes(&bstrProperty);
				if (SUCCEEDED(hr) && bstrProperty != updatedFwRule.getICMPTypeCode())
				{
					hr = fwRule->put_IcmpTypesAndCodes(updatedFwRule.getICMPTypeCode());
					if (FAILED(hr))
					{
						wprintf(L"put_IcmpTypesAndCodes failed: 0x%08lx\n", hr);
					}
				}
				bstrProperty.Empty(); // Clear the BSTR
			}
			else
			{
				hr = fwRule->put_LocalPorts(nullptr);
				hr = fwRule->put_RemotePorts(nullptr);
				hr = fwRule->put_IcmpTypesAndCodes(nullptr);
				hr = fwRule->put_Protocol(updatedFwRule.getProtocol());
				if (FAILED(hr))
				{
					wprintf(L"put_Protocol failed: 0x%08lx\n", hr);
				}
			}
		}
		else
		{
			if (lProperty == IP_PROTOCOL_TCP or lProperty == IP_PROTOCOL_UDP)
			{
				hr = fwRule->get_LocalPorts(&bstrProperty);
				if (SUCCEEDED(hr) && bstrProperty != updatedFwRule.getLocalPorts())
				{
					hr = fwRule->put_LocalPorts(updatedFwRule.getLocalPorts());
					if (FAILED(hr))
					{
						wprintf(L"put_LocalPorts failed: 0x%08lx\n", hr);
					}
				}
				bstrProperty.Empty(); // Clear the BSTR

				hr = fwRule->get_RemotePorts(&bstrProperty);
				if (SUCCEEDED(hr) && bstrProperty != updatedFwRule.getRemotePorts())
				{
					hr = fwRule->put_RemotePorts(updatedFwRule.getRemotePorts());
					if (FAILED(hr))
					{
						wprintf(L"put_RemotePorts failed: 0x%08lx\n", hr);
					}
				}
				bstrProperty.Empty(); // Clear the BSTR
			}
			else if (lProperty == IP_PROTOCOL_ICMP4 or lProperty == IP_PROTOCOL_ICMP6)
			{
				hr = fwRule->get_IcmpTypesAndCodes(&bstrProperty);
				if (SUCCEEDED(hr) && bstrProperty != updatedFwRule.getICMPTypeCode())
				{
					hr = fwRule->put_IcmpTypesAndCodes(updatedFwRule.getICMPTypeCode());
					if (FAILED(hr))
					{
						wprintf(L"put_IcmpTypesAndCodes failed: 0x%08lx\n", hr);
					}
				}
				bstrProperty.Empty(); // Clear the BSTR
			}
		}
	}

	hr = fwRule->get_LocalAddresses(&bstrProperty);
	if (SUCCEEDED(hr) && bstrProperty != updatedFwRule.getLocalAddresses())
	{
		hr = fwRule->put_LocalAddresses(updatedFwRule.getLocalAddresses());
		if (FAILED(hr))
		{
			wprintf(L"put_LocalAddresses failed: 0x%08lx\n", hr);
		}
	}
	bstrProperty.Empty(); // Clear the BSTR

	hr = fwRule->get_RemoteAddresses(&bstrProperty);
	if (SUCCEEDED(hr) && bstrProperty != updatedFwRule.getRemoteAddresses())
	{
		hr = fwRule->put_RemoteAddresses(updatedFwRule.getRemoteAddresses());
		if (FAILED(hr))
		{
			wprintf(L"put_RemoteAddresses failed: 0x%08lx\n", hr);
		}
	}
	bstrProperty.Empty(); // Clear the BSTR

	hr = fwRule->get_InterfaceTypes(&bstrProperty);
	if (SUCCEEDED(hr) && bstrProperty != updatedFwRule.getInterfaceTypes())
	{
		hr = fwRule->put_InterfaceTypes(updatedFwRule.getInterfaceTypes());
		if (FAILED(hr))
		{
			wprintf(L"put_InterfaceTypes failed: 0x%08lx\n", hr);
		}
	}
	bstrProperty.Empty(); // Clear the BSTR

	hr = fwRule->get_Enabled(&enabled);
	if (SUCCEEDED(hr) && enabled != updatedFwRule.getEnabled())
	{
		hr = fwRule->put_Enabled(updatedFwRule.getEnabled());
		if (FAILED(hr))
		{
			wprintf(L"put_Enabled failed: 0x%08lx\n", hr);
		}
	}

	hr = fwRule->get_EdgeTraversal(&enabled);
	if (SUCCEEDED(hr) && enabled != updatedFwRule.getEdgeTraversal())
	{
		hr = fwRule->put_EdgeTraversal(updatedFwRule.getEdgeTraversal());
		if (FAILED(hr))
		{
			wprintf(L"put_EdgeTraversal failed: 0x%08lx\n", hr);
		}
	}
}

void UpdateFwRule(CComPtr<INetFwRule> fwRule, int position)
{
	FwRule updatedFwRule;
	DumpFWRulesInCollection(fwRule, position);
	UpdatePropertiesDialog(fwRule);
}

void SetFwRuleInitialValues(CComPtr<INetFwRule> fwRule, FwRule& updatedRule)
{
	HRESULT hr = S_OK;

	CComBSTR bstrProperty;
	LONG lProperty = 0;
	long lProfilesBitMask = 0;
	VARIANT_BOOL boolProperty;
	NET_FW_RULE_DIRECTION direction;
	NET_FW_ACTION action;

	hr = fwRule->get_Name(&bstrProperty);
	if (SUCCEEDED(hr))
	{
		updatedRule.setName(bstrProperty);
	}

	hr = fwRule->get_Description(&bstrProperty);
	if (SUCCEEDED(hr))
	{
		updatedRule.setDescription(bstrProperty);
	}

	hr = fwRule->get_Grouping(&bstrProperty);
	if (SUCCEEDED(hr))
	{
		updatedRule.setGroup(bstrProperty);
	}

	hr = fwRule->get_ApplicationName(&bstrProperty);
	if (SUCCEEDED(hr))
	{
		updatedRule.setApplicationName(bstrProperty);
	}

	hr = fwRule->get_LocalPorts(&bstrProperty);
	if (SUCCEEDED(hr))
	{
		updatedRule.setLocalPorts(bstrProperty);
	}

	hr = fwRule->get_RemotePorts(&bstrProperty);
	if (SUCCEEDED(hr))
	{
		updatedRule.setRemotePorts(bstrProperty);
	}

	hr = fwRule->get_IcmpTypesAndCodes(&bstrProperty);
	if (SUCCEEDED(hr))
	{
		updatedRule.setICMPTypeCode(bstrProperty);
	}

	hr = fwRule->get_Protocol(&lProperty);
	if (SUCCEEDED(hr))
	{
		updatedRule.setProtocol(lProperty);
	}

	hr = fwRule->get_Profiles(&lProfilesBitMask);
	if (SUCCEEDED(hr))
	{
		updatedRule.setProfilesBitMask(lProfilesBitMask);
	}

	hr = fwRule->get_Direction(&direction);
	if (SUCCEEDED(hr))
	{
		updatedRule.setDirection(direction);
	}

	hr = fwRule->get_Action(&action);
	if (SUCCEEDED(hr))
	{
		updatedRule.setAction(action);
	}

	hr = fwRule->get_Enabled(&boolProperty);
	if (SUCCEEDED(hr))
	{
		updatedRule.setEnabled(boolProperty);
	}

	hr = fwRule->get_EdgeTraversal(&boolProperty);
	if (SUCCEEDED(hr))
	{
		updatedRule.setEdgeTraversal(boolProperty);
	}

	hr = fwRule->get_LocalAddresses(&bstrProperty);
	if (SUCCEEDED(hr))
	{
		updatedRule.setLocalAddresses(bstrProperty);
	}

	hr = fwRule->get_RemoteAddresses(&bstrProperty);
	if (SUCCEEDED(hr))
	{
		updatedRule.setRemoteAddresses(bstrProperty);
	}

	hr = fwRule->get_InterfaceTypes(&bstrProperty);
	if (SUCCEEDED(hr))
	{
		updatedRule.setInterfaceTypes(bstrProperty);
	}
}

void UpdatePropertiesDialog(CComPtr<INetFwRule> fwRule)
{
	FwRule updatedFwRule = FwRule();
	LONG lProtocol;
	VARIANT_BOOL enabled;
	string strEnabled;
	HRESULT hr = S_OK;

	SetFwRuleInitialValues(fwRule, updatedFwRule);

	hr = fwRule->get_Protocol(&lProtocol);
	if(FAILED(hr)) 
	{
		wprintf(L"get_Protocol failed: 0x%08lx\n", hr);
		wprintf(L"UpdateFwRule failed\n");
		return;
	}

	cout << "Change Application Name (y/n): ";
	if (EnterYesNoInput() == 'y') 
	{
		updatedFwRule.setApplicationName(EnterFwRuleAppName());
	}

	cout << "Change Protocol (y/n): ";
	if (EnterYesNoInput() == 'y') 
	{
		lProtocol = EnterFwRuleProtocolNumber();
		updatedFwRule.setProtocol(lProtocol);
	}

	if (lProtocol == IP_PROTOCOL_TCP or lProtocol == IP_PROTOCOL_UDP)
	{
		cout << "Change Local Ports (y/n): ";
		if (EnterYesNoInput() == 'y') 
		{
			updatedFwRule.setLocalPorts(EnterFwRulePorts(true, lProtocol == IP_PROTOCOL_TCP));
		}

		cout << "Change Remote Ports (y/n): ";
		if (EnterYesNoInput() == 'y') 
		{
			updatedFwRule.setRemotePorts(EnterFwRulePorts(false, lProtocol == IP_PROTOCOL_TCP));
		}
	}

	else if (lProtocol == IP_PROTOCOL_ICMP4 or lProtocol == IP_PROTOCOL_ICMP6)
	{
		cout << "Change ICMP Types (y/n): ";
		if (EnterYesNoInput() == 'y')
		{
			updatedFwRule.setICMPTypeCode(EnterFwRuleICMPTypes(lProtocol == 1));
		}
	}

	cout << "Change Direction (y/n): ";
	if (EnterYesNoInput() == 'y')
	{
		updatedFwRule.setDirection(EnterFwRuleDirection());
	}

	cout << "Change Action (y/n): ";
	if (EnterYesNoInput() == 'y')
	{
		updatedFwRule.setAction(EnterFwRuleAction());
	}

	cout << "Change Profiles (y/n): ";
	if (EnterYesNoInput() == 'y')
	{
		updatedFwRule.setProfilesBitMask(EnterFwRuleProfiles());
	}

	cout << "Change Name (y/n): ";
	if (EnterYesNoInput() == 'y')
	{
		updatedFwRule.setName(EnterFwRuleName());
	}

	cout << "Change Description (y/n): ";
	if (EnterYesNoInput() == 'y')
	{
		updatedFwRule.setDescription(EnterFwRuleDescription());
	}

	hr = fwRule->get_Enabled(&enabled);
	if (SUCCEEDED(hr)) 
	{
		strEnabled = enabled ? "enabled" : "disabled";
		cout << "Firewall rule is currently " << strEnabled << ". Change it (y/n): ";
		if (EnterYesNoInput() == 'y')
		{
			updatedFwRule.setEnabled(~enabled);
		}
	}

	hr = fwRule->get_EdgeTraversal(&enabled);
	if (SUCCEEDED(hr))
	{
		strEnabled = enabled ? "enabled" : "disabled";
		cout << "Edge Traversal is currently " << strEnabled << ". Change it (y/n): ";
		if (EnterYesNoInput() == 'y')
		{
			updatedFwRule.setEdgeTraversal(~enabled);
		}
	}

	cout << "Change Interface types (y/n): ";
	if (EnterYesNoInput() == 'y')
	{
		updatedFwRule.setInterfaceTypes(EnterFwRuleInterfaceTypes());
	}

	//TODO: Change Local/Remote Addresses

	cout << "Save changes (y/n): ";
	if (EnterYesNoInput() == 'y') 
	{
		UpdateFwRuleChanges(fwRule, updatedFwRule);
	}
}