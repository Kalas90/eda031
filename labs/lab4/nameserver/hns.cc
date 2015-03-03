#include <vector>
#include <algorithm>
#include <functional>
#include "hns.h"

using namespace std;
HNS::HNS(size_t s) : vec(vector<vector<entry>>(s)), size(s) {
}

void HNS::insert(const HostName& host, const IPAddress& ip) {
    size_t pos = hash<string>()(host) % size;
    vector<entry> *inner = &vec[pos];

    if (inner->end() != find_if(inner->begin(),
                                inner->end(),
                                [&host](const entry &e){return e.first == host;}))
        return; // if already exists, just pretend to do something

    inner->push_back(make_pair(host, ip));
}

bool HNS::remove(const HostName& host) {
    size_t pos = hash<string>()(host) % size;
    vector<entry> *inner = &vec[pos];

    auto it = find_if(inner->begin(),
                      inner->end(),
                      [&host](const entry &e){return e.first == host;});
    if (it != inner->end()) {
        inner->erase(it);
        return true;
    } else {
        return false;
    }
}

IPAddress HNS::lookup(const HostName& host) const {
    size_t pos = hash<string>()(host) % size;
    vector<entry> inner = vec[pos];

    auto it = find_if(inner.begin(),
                      inner.end(),
                      [&host](const entry &e){return e.first == host;});
    return it != inner.end() ? it->second : NON_EXISTING_ADDRESS;
}


askjadaksjdljsadjsdasdlkaskdj