#ifndef UMNS_H
#define UMNS_H

#include <vector>
#include <unordered_map>

#include "nameserverinterface.h"

using entry = std::pair<HostName, IPAddress>;

class UMNS : public NameServerInterface {
public:
    void insert(const HostName&, const IPAddress&);
    bool remove(const HostName&);
    IPAddress lookup(const HostName&) const;

private:
    std::unordered_map<HostName, IPAddress> map;
};


#endif