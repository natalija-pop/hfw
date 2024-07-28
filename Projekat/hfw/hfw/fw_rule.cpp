#include "fw_rule.h"

FwRule::FwRule() :
    bstrName(nullptr),
    bstrDescription(nullptr),
    bstrGroup(nullptr),
    bstrApplicationName(nullptr),
    bstrLocalPorts(nullptr),
    bstrRemotePorts(nullptr),
    bstrICMPTypeCode(nullptr),
    lProtocol(-1),
	lProfilesBitMask(0),
    direction(NET_FW_RULE_DIR_IN),
    action(NET_FW_ACTION_BLOCK)
{}

BSTR FwRule::getName() const { return bstrName; }
BSTR FwRule::getDescription() const { return bstrDescription; }
BSTR FwRule::getGroup() const { return bstrGroup; }
BSTR FwRule::getApplicationName() const { return bstrApplicationName; }
BSTR FwRule::getLocalPorts() const { return bstrLocalPorts; }
BSTR FwRule::getRemotePorts() const { return bstrRemotePorts; }
BSTR FwRule::getICMPTypeCode() const { return bstrICMPTypeCode; }
LONG FwRule::getProtocol() const { return lProtocol; }
LONG FwRule::getProfilesBitMask() const{ return lProfilesBitMask; }

NET_FW_RULE_DIRECTION FwRule::getDirection() const { return direction; }
NET_FW_ACTION FwRule::getAction() const { return action; }

void FwRule::setName(BSTR name) { bstrName = name; }
void FwRule::setDescription(BSTR description) { bstrDescription = description; }
void FwRule::setGroup(BSTR group) { bstrGroup = group; }
void FwRule::setApplicationName(BSTR applicationName) { bstrApplicationName = applicationName; }
void FwRule::setLocalPorts(BSTR localPorts) { bstrLocalPorts = localPorts; }
void FwRule::setRemotePorts(BSTR remotePorts) { bstrRemotePorts = remotePorts; }
void FwRule::setICMPTypeCode(BSTR icmpTypeCode) { bstrICMPTypeCode = icmpTypeCode; }
void FwRule::setProtocol(LONG protocol) { lProtocol = protocol; }

void FwRule::setProfilesBitMask(LONG bitmask)
{
    lProfilesBitMask = bitmask;
}

void FwRule::setDirection(NET_FW_RULE_DIRECTION newDirection) { direction = newDirection; }
void FwRule::setAction(NET_FW_ACTION newAction) { action = newAction; }