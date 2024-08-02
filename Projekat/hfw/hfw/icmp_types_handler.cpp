#include "icmp_types_handler.h"

#include <atlcomcli.h>
#include <iostream>

#include "string_helper.h"
#include <regex>
#include <string>
#include <set>

using namespace std;

string ICMPTypesCodesBuilder(const std::string& str)
{
    const string strCopy = str.back() == ',' ? str : str + ',';
    string inputCopy;
    for (const char& i : strCopy)
    {
        if(i == ',')
        {
            inputCopy += ":*,";
        }
        else
        {
            inputCopy += i;
        }
    }
    inputCopy = inputCopy.back() == ',' ? inputCopy.substr(0, inputCopy.length() - 1) : inputCopy;
    return inputCopy;
}

bool IsICMPValid(const std::string& str)
{
    string inputCopy = str.back() == ',' ? str : str + ',';
    if (!regex_match(inputCopy, std::basic_regex<char>("([0-9]+,)+"))) return false;
    
    inputCopy.erase(std::remove_if(inputCopy.begin(), inputCopy.end(), [](char c) {
        return c == ',';
    }), inputCopy.end());

    const set<char> uniquenessChecker(inputCopy.begin(), inputCopy.end());
    return uniquenessChecker.size() == inputCopy.size();
}

void DisplayICMPTypes(bool isVersion4)
{
    const string sVersionMessage = isVersion4 ? "ICMPv4" : "ICMPv6";
    cout << "ICMP TYPES & CODES\n";
    cout << "This is a list of " << sVersionMessage << " type values:\n";

    if (isVersion4) 
    {
        for (const auto& msg : icmp_types_constants::typesV4) 
        {
            cout << msg << "\n";
        }
    }
    else 
    {
        for (const auto& msg : icmp_types_constants::typesV6) 
        {
            cout << msg << "\n";
        }
    }
    cout << "Enter number or numbers from the list separated with comma (,). For all enter * : ";
}

BSTR EnterICMPTypes(bool isVersion4)
{
    string sIcmpTypes;
    bool areTypesValid;

    DisplayICMPTypes(isVersion4);

	do
    {
        cin.clear();
        getline(std::cin, sIcmpTypes);
        if (sIcmpTypes == "*") return CComBSTR(sIcmpTypes.c_str()).Detach();
        areTypesValid = IsICMPValid(sIcmpTypes);
        if (!areTypesValid)
        {
            cout << "Invalid enter for ICMP types. Try again.\n";
        }
    }
    while (!areTypesValid);
    cout << "Selected ICMP types: " << ICMPTypesCodesBuilder(sIcmpTypes) << "\n";
    return CComBSTR(ICMPTypesCodesBuilder(sIcmpTypes).c_str()).Detach();
}