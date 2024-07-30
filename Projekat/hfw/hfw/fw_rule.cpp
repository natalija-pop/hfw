#include "fw_rule.h"

FwRule::FwRule() : bstrName(nullptr),
bstrDescription(nullptr),
bstrGroup(nullptr),
bstrApplicationName(nullptr),
bstrLocalPorts(nullptr),
bstrLocalAddresses(nullptr),
bstrRemotePorts(nullptr),
bstrRemoteAddresses(nullptr),
bstrICMPTypeCode(nullptr),
bstrInterfaceTypes(nullptr),
lProtocol(256),
lProfilesBitMask(0),
bEnabled(VARIANT_FALSE),
bEdgeTraversal(VARIANT_FALSE),
direction(NET_FW_RULE_DIR_IN),
action(NET_FW_ACTION_BLOCK) 
{
}

BSTR FwRule::getName() const { return bstrName; }
BSTR FwRule::getDescription() const { return bstrDescription; }
BSTR FwRule::getGroup() const { return bstrGroup; }
BSTR FwRule::getApplicationName() const { return bstrApplicationName; }
BSTR FwRule::getLocalPorts() const { return bstrLocalPorts; }
BSTR FwRule::getRemotePorts() const { return bstrRemotePorts; }
BSTR FwRule::getICMPTypeCode() const { return bstrICMPTypeCode; }
LONG FwRule::getProtocol() const { return lProtocol; }
LONG FwRule::getProfilesBitMask() const { return lProfilesBitMask; }
NET_FW_RULE_DIRECTION FwRule::getDirection() const { return direction; }
NET_FW_ACTION FwRule::getAction() const { return action; }
VARIANT_BOOL FwRule::getEnabled() const { return bEnabled; }
VARIANT_BOOL FwRule::getEdgeTraversal() const { return bEdgeTraversal; }
BSTR FwRule::getLocalAddresses() const { return bstrLocalAddresses; }
BSTR FwRule::getRemoteAddresses() const { return bstrRemoteAddresses; }
BSTR FwRule::getInterfaceTypes() const { return bstrInterfaceTypes; }

void FwRule::setName(BSTR name) { bstrName = name; }
void FwRule::setDescription(BSTR description) { bstrDescription = description; }
void FwRule::setGroup(BSTR group) { bstrGroup = group; }
void FwRule::setApplicationName(BSTR applicationName) { bstrApplicationName = applicationName; }
void FwRule::setLocalPorts(BSTR localPorts) { bstrLocalPorts = localPorts; }
void FwRule::setRemotePorts(BSTR remotePorts) { bstrRemotePorts = remotePorts; }
void FwRule::setICMPTypeCode(BSTR icmpTypeCode) { bstrICMPTypeCode = icmpTypeCode; }
void FwRule::setProtocol(LONG protocol) { lProtocol = protocol; }
void FwRule::setProfilesBitMask(LONG bitmask) { lProfilesBitMask = bitmask; }
void FwRule::setDirection(NET_FW_RULE_DIRECTION newDirection) { direction = newDirection; }
void FwRule::setAction(NET_FW_ACTION newAction) { action = newAction; }
void FwRule::setEnabled(VARIANT_BOOL enabled) { bEnabled = enabled; }
void FwRule::setEdgeTraversal(VARIANT_BOOL edgeTraversal) { bEdgeTraversal = edgeTraversal; }
void FwRule::setLocalAddresses(BSTR localAddresses) { bstrLocalAddresses = localAddresses; }
void FwRule::setRemoteAddresses(BSTR remoteAddresses) { bstrRemoteAddresses = remoteAddresses; }
void FwRule::setInterfaceTypes(BSTR interfaceTypes) { bstrInterfaceTypes = interfaceTypes; }