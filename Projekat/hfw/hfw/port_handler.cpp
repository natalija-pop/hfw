#include "port_handler.h"
#include "string_helper.h"
#include "enums.h"
#include <algorithm>
#include <atlcomcli.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace  std;

bool IsPortOutOfRange(const int portNumber)
{
    return portNumber < 0 or portNumber > MAX_PORT_NUMBER;
}

bool IsPortRangeValid(const int firstPort, const int secondPort)
{
    return !IsPortOutOfRange(firstPort) and !IsPortOutOfRange(secondPort) and firstPort < secondPort;
}

bool ValidatePortRange(const string& ports)
{
    vector<string> dashSeparatedPorts = SplitString(ports, '-');
    if (dashSeparatedPorts.empty() or dashSeparatedPorts.size() > 2) return false;

    if (dashSeparatedPorts.size() == 1) //Example 124, 658972
    {
        try
        {
            int port = stoi(dashSeparatedPorts[0]);
            if (IsPortOutOfRange(port)) return false;
        }
        catch (const invalid_argument& e)
        {
            cout << "Invalid argument: " << e.what() << endl;
            return false;
        }
        catch (const out_of_range& e)
        {
            cout << "Out of range: " << e.what() << endl;
            return false;
        }
        return true;
    }

    if (dashSeparatedPorts.size() == 2) //Example: 1234-12345, -1234, 4582-
    {
        try
        {
            int firstPort = stoi(dashSeparatedPorts[0]);
            int secondPort = stoi(dashSeparatedPorts[1]);
            if (!IsPortRangeValid(firstPort, secondPort)) return false;
        }
        catch (const invalid_argument& e)
        {
            cout << "Invalid argument: " << e.what() << endl;
            return false;
        }
        catch (const out_of_range& e)
        {
            cout << "Out of range: " << e.what() << endl;
            return false;
        }
    }
    return true;
}

bool AreSpecificPortsValid(const string& ports)
{
    const string noWhiteSpacePorts = RemoveWhiteSpaces(ports);

    vector<string> commaSeparatedPorts = SplitString(noWhiteSpacePorts, ',');
    if (commaSeparatedPorts.empty()) return false;

	for (const auto& part : commaSeparatedPorts)
    {
        if (!ValidatePortRange(part)) return false;
    }
    return true;
}

string EnterFwRuleSpecificPorts()
{
    string sPorts;
    bool arePortsValid;
    cout << "Enter single port, multiple ports or range of ports\n"
        << "Examples: 80, 443, 45125-65535\n";
    do
    {
        cin.clear();
        getline(std::cin, sPorts);
        arePortsValid = AreSpecificPortsValid(sPorts);
        if (!arePortsValid)
        {
            cout << "Invalid enter for ports. Try again.\n";
        }
    } while (!arePortsValid);
    return RemoveWhiteSpaces(sPorts);
}

void DisplayPortOptions(bool isLocal, bool isProtocolTCP)
{
	const string sPortsMessage = isLocal ? "local" : "remote";
	cout << "PORTS\n";
	cout << "This is the list of allowed " << sPortsMessage <<  " port values:\n"
		<< "0. All ports\n"
		<< "1. Specific ports\n";

	if(isLocal)
	{
		if (isProtocolTCP)
		{
			cout << "2. RPC Dynamic ports\n"
				<< "3. RPC Specific mapper\n"
				<< "4. IPHTTPS\n";
		}
		else
		{
			cout << "2. Edge traversal\n"
				<< "3. PlayTo Discovery\n";
		}
	}
	cout << "Enter the number from the list\n";
}

BSTR EnterPorts(bool isLocal, bool isProtocolTCP)
{
    string ports;
    int input = -1;
    const int maxInput = !isLocal ? 1 : isProtocolTCP ? 4 : 3;

    DisplayPortOptions(isLocal, isProtocolTCP);
    do
    {
        cin >> input;
        switch (input)
        {
        case 0: ports = "*"; break;
        case 1: ports = EnterFwRuleSpecificPorts(); break;
        case 2: 
	        {
				if(!isLocal)
				{
	                cout << "Invalid enter. Try again.\n"; break;
				}
	            ports = isProtocolTCP ? to_string(RPC_DYNAMIC_PORTS) : to_string(EDGE_TRAVERSAL); break;
	        }
        case 3:
	        {
				if (!isLocal)
				{
					cout << "Invalid enter. Try again.\n"; break;
				}
                ports = isProtocolTCP ? to_string(RPC_ENDPOINT_MAPPER): to_string(PLAY_TO_DISCOVERY); break;
	        }
        case 4:
	        {
                if(isLocal and isProtocolTCP)
                {
                    ports = to_string(IPHTTPS); break;
                }
        		cout << "Invalid enter. Try again.\n"; break;
	        }
        default: cout << "Invalid enter. Try again.\n";
        }
    }
    while (input < 0 or input > maxInput);
	return CComBSTR(ports.c_str()).Detach();
}