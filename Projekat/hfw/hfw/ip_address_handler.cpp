#include "ip_address_handler.h"
#include "string_helper.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <iostream>
#include <vector>


#pragma comment(lib, "Ws2_32.lib")

using namespace std;

bool isValidIPv4(const std::string& ipAddress) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
    return result == 1;
}

bool isValidIPv6(const std::string& ipAddress) {
    struct sockaddr_in6 sa6;
    int result = inet_pton(AF_INET6, ipAddress.c_str(), &(sa6.sin6_addr));
    return result == 1;
}

bool isIPAddressValid(const std::string& ipAddress)
{
    return isValidIPv4(ipAddress) || isValidIPv6(ipAddress);
}

bool isIPAddressRangeValid(const std::string& ipAddress) 
{
    const vector<string> dashSeparated = SplitString(ipAddress, '-');
    if (dashSeparated.empty() || dashSeparated.size() > 2) return false;

    if(dashSeparated.size() == 1) // This is just one IP Address
    {
        return isIPAddressValid(dashSeparated[0]);
    }
    for (int i = 0; i < dashSeparated.size(); i++) 
    {
        if (!isIPAddressValid(dashSeparated[i])) return false;
    }
    return true;
}

std::string EnterIPAddress()
{
    WSADATA wsaData;
    int wsaStartupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaStartupResult != 0) {
        std::cerr << "WSAStartup failed: " << wsaStartupResult << std::endl;
        return "";
    }
    string ipAddressRange;
    string ipAddress;

    cout << "IP ADDRESSES\n";
    cout << "Enter IP address (y/n): ";
    while (EnterYesNoInput() == 'y')
    {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cout << "Enter IP Address you want to add or enter range of addresses: ";
        getline(std::cin, ipAddress);
        if (!isIPAddressRangeValid(ipAddress)) 
        {
            cout << "Please enter valid IP Address! Try again.\n";
        }
        else
        {
            if (!ipAddressRange.empty())
            {
                ipAddressRange += ",";
            }
            ipAddressRange += ipAddress;
        }
        cout << "Enter IP address (y/n): ";
    }
    WSACleanup();
    return ipAddressRange;
}