#include <vector>
#include <algorithm>
#include "mns.h"

using namespace std;


void MNS::insert(const HostName& host, const IPAddress& ip) {
    map.insert({host, ip});
}

bool MNS::remove(const HostName& host) {
    return map.erase(host);
}

IPAddress MNS::lookup(const HostName& host) const {
    auto it = map.find(host);
    return it != map.end() ? it->second : NON_EXISTING_ADDRESS;
}