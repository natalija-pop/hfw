#pragma once
#include <netfw.h>
#include <string>
#include <vector>


HRESULT WFCOMInitialize(INetFwPolicy2** ppNetFwPolicy2);
void    WFCOMCleanup(INetFwPolicy2* pNetFwPolicy2);

LONG EnterFwRuleProtocolNumber();
LONG EnterFwRuleProfiles();
NET_FW_RULE_DIRECTION EnterFwRuleDirection();
NET_FW_ACTION EnterFwRuleAction();
BSTR EnterFwRuleName();
BSTR EnterFwRuleDescription();
BSTR EnterFwRuleAppName();
BSTR EnterFwRuleInterfaceTypes();
void EnterInterface(const std::string interfaceType, std::string& sInterfaces);
BSTR EnterFwRuleICMPTypes(bool isVersion4 = true);
BSTR EnterFwRulePorts(bool isLocal, bool isProtocolTCP);

namespace protocol_numbers
{
	const std::vector<std::string> protocols{
	    "0. HOPOPT",
	    "1. ICMPv4",
	    "2. IGMP",
	    "6. TCP",
	    "17. UDP",
	    "41. IPv6",
	    "43. IPv6-Route",
	    "44. IPv6-Frag",
	    "47. GRE",
	    "58. ICMPv6",
	    "59. IPv6-NoNxt",
	    "60. IPv6-Opts",
	    "112. VRRP",
	    "113. PGM",
	    "115. L2TP",
		"256. Any"
	};
}