#pragma once

#include <netfw.h>

class FwRule
{
    BSTR bstrName;
    BSTR bstrDescription;
    BSTR bstrGroup;
    BSTR bstrApplicationName;
    BSTR bstrLocalPorts;
    BSTR bstrRemotePorts;
    BSTR bstrICMPTypeCode; // Only for ICMP rule

    LONG lProtocol;
    LONG lProfilesBitMask;

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
};