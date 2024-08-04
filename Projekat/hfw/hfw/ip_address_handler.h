#pragma once

#include <string>

bool isIPAddressRangeValid(const std::string& ipAddress);
bool isIPAddressValid(const std::string& ipAddress);
bool isValidIPv4(const std::string& ipAddress);
bool isValidIPv6(const std::string& ipAddress);
std::string EnterIPAddress();
