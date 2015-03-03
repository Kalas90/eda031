#include <vector>
#include <algorithm>
#include "umns.h"

using namespace std;


void UMNS::insert(const HostName& host, const IPAddress& ip) {
    map.insert({host, ip});
}

bool UMNS::remove(const HostName& host) {
    return map.erase(host);
}

IPAddress UMNS::lookup(const HostName& host) const {
    auto it = map.find(host);
    return it != map.end() ? it->second : NON_EXISTING_ADDRESS;
}