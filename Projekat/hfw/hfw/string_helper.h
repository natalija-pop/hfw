#pragma once

#include <atlcomcli.h>
#include <string>
#include <vector>
#include <wtypes.h>

std::vector<std::string> SplitString(const std::string& str, const char delimiter);
std::string RemoveWhiteSpaces(const std::string& str);
CComBSTR GenerateGUIDString();
char EnterYesNoInput();
BSTR ConvertStringToBSTR(const std::string& input);