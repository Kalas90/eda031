#ifndef HNS_H
#define HNS_H

#include <vector>

#include "nameserverinterface.h"

using entry = std::pair<HostName, IPAddress>;

class HNS : public NameServerInterface {
public:
    HNS(size_t size);
    void insert(const HostName&, const IPAddress&);
    bool remove(const HostName&);
    IPAddress lookup(const HostName&) const;

private:
    size_t size;
    std::vector<std::vector<entry>> vec;
};


#endif