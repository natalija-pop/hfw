#pragma once
#include <icftypes.h>
#include <string>
#include <vector>

void DisplayICMPTypes(bool isVersion4 = true);
BSTR EnterICMPTypes(bool isVersion4 = true);
std::string ICMPTypesCodesBuilder(const std::string& str);
bool IsICMPValid(const std::string& str);

namespace icmp_types_constants
{
    const std::vector<std::string> typesV4 = {
        "2. PacketTooBig",
        "3. Destination Unreachable",
        "4. Source Quench",
        "5. Redirect",
        "8. Echo Request",
        "9. Router Advertisement",
        "10. Router Solicitation",
        "11. Time Exceeded",
        "12. Parameter Problem",
        "13. Timestamp Request",
        "17. Address Mask Request"
    };
    const std::vector<std::string> typesV6 = {
        "1. Destination Unreachable",
        "2. PacketTooBig",
        "3. Time Exceeded",
        "4. Parameter Problem",
        "128. EchoRequest",
        "130. Multicast Listener Query",
        "131. Multicast Listener Report",
        "132. Multicast Listener Done",
        "133. Router Advertisement",
        "134. Router Solicitation",
        "135. Neighbor Discovery",
        "136. Neighbor Advertisement",
        "137. Redirect",
        "143. Multicast Listener v2"
    };
}