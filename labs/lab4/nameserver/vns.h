#ifndef VNS_H
#define VNS_H

#include <vector>

#include "nameserverinterface.h"

using entry = std::pair<HostName, IPAddress>;

class VNS : public NameServerInterface {
public:
    void insert(const HostName&, const IPAddress&);
    bool remove(const HostName&);
    IPAddress lookup(const HostName&) const;

private:
    std::vector<entry> vec;
};


#endif