#pragma once

#include <netfw.h>

class FwRule
{
    BSTR bstrName;
    BSTR bstrDescription;
    BSTR bstrGroup;
    BSTR bstrApplicationName;
    BSTR bstrLocalPorts;
    BSTR bstrLocalAddresses;
    BSTR bstrRemotePorts;
    BSTR bstrRemoteAddresses;
    BSTR bstrICMPTypeCode; // Only for ICMP rule
    BSTR bstrInterfaceTypes;

    LONG lProtocol;
    LONG lProfilesBitMask;

    VARIANT_BOOL bEnabled;
    VARIANT_BOOL bEdgeTraversal;

    VARIANT interfaceArray;

    NET_FW_RULE_DIRECTION direction;
    NET_FW_ACTION action;

public:
    FwRule();

    BSTR getName() const;
    BSTR getDescription() const;
    BSTR getGroup() const;
    BSTR getApplicationName() const;
    BSTR getLocalPorts() const;
    BSTR getRemotePorts() const;
    BSTR getICMPTypeCode() const;
    LONG getProtocol() const;
    LONG getProfilesBitMask() const;
    NET_FW_RULE_DIRECTION getDirection() const;
    NET_FW_ACTION getAction() const;
    VARIANT_BOOL getEnabled() const;
    VARIANT_BOOL getEdgeTraversal() const;
    BSTR getLocalAddresses() const;
    BSTR getRemoteAddresses() const;
    BSTR getInterfaceTypes() const;
    BSTR getGrouping() const;

    void setName(BSTR name);
    void setDescription(BSTR description);
    void setGroup(BSTR group);
    void setApplicationName(BSTR applicationName);
    void setLocalPorts(BSTR localPorts);
    void setRemotePorts(BSTR remotePorts);
    void setICMPTypeCode(BSTR icmpTypeCode);
    void setProtocol(LONG protocol);
    void setProfilesBitMask(LONG bitmask);
    void setDirection(NET_FW_RULE_DIRECTION direction);
    void setAction(NET_FW_ACTION action);
    void setEnabled(VARIANT_BOOL enabled);
    void setEdgeTraversal(VARIANT_BOOL edgeTraversal);
    void setLocalAddresses(BSTR localAddresses);
    void setRemoteAddresses(BSTR remoteAddresses);
    void setInterfaceTypes(BSTR interfaceTypes);
};