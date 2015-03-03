#include <vector>
#include <algorithm>
#include "vns.h"

using namespace std;


void VNS::insert(const HostName& host, const IPAddress& ip) {
    vec.push_back(make_pair(host, ip));
}

bool VNS::remove(const HostName& host) {
    auto it = find_if(vec.begin(), vec.end(), [&host](const entry &e){return e.first == host;});
    if (it != vec.end()) {
        vec.erase(it);
        return true;
    } else {
        return false;
    }
}

IPAddress VNS::lookup(const HostName& host) const {
    auto it = find_if(vec.begin(), vec.end(), [&host](const entry &e){return e.first == host;});
    if (it != vec.end()) {
        return it->second;
    } else {
        return NON_EXISTING_ADDRESS;
    }
}