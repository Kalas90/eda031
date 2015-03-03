#ifndef MNS_H
#define MNS_H

#include <vector>
#include <map>

#include "nameserverinterface.h"

using entry = std::pair<HostName, IPAddress>;

class MNS : public NameServerInterface {
public:
    void insert(const HostName&, const IPAddress&);
    bool remove(const HostName&);
    IPAddress lookup(const HostName&) const;

private:
    std::map<HostName, IPAddress> map;
};


#endif