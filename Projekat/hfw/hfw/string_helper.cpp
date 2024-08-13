#include "string_helper.h"
#include <algorithm>
#include <atlcomcli.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <regex>

using namespace std;

vector<string> SplitString(const string& str, const char delimiter)
{
    vector<string> vec;
    stringstream ss(str);
    string word;
    while (!ss.eof())
    {
        getline(ss, word, delimiter);
        vec.push_back(word);
    }
    return vec;
}

string RemoveWhiteSpaces(const string& str)
{
    string noWhiteSpaceStr;
    remove_copy_if(str.begin(), str.end(), back_inserter(noWhiteSpaceStr), [](unsigned char character)
        {
            return isspace(character);
        });
    return noWhiteSpaceStr;
}

//Reference: https://gist.github.com/manassra/28c3d9270b18dbfcf4ad
CComBSTR GenerateGUIDString()
{
    GUID guid;
    if (FAILED(CoCreateGuid(&guid)))
    {
        return nullptr;
    }
    stringstream stream;
    stream << std::hex << std::uppercase
        << std::setw(8) << std::setfill('0') << guid.Data1
        << "-" << std::setw(4) << std::setfill('0') << guid.Data2
        << "-" << std::setw(4) << std::setfill('0') << guid.Data3
        << "-";
    for (int i = 0; i < sizeof(guid.Data4); ++i)
    {
        if (i == 2)
            stream << "-";
        stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(guid.Data4[i]);
    }
    return ConvertStringToBSTR(stream.str());
}

char EnterYesNoInput()
{
    char input;
    do
    {
        cin >> input;
        if (input != 'n' and input != 'y')
        {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "Invalid enter. Try again.\n";
        }

    } while (input != 'n' and input != 'y');
    return input;
}

BSTR ConvertStringToBSTR(const string& input)
{
    return CComBSTR(input.c_str()).Detach();
}