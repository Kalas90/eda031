#ifndef BIT_BUFFER_H
#define BIT_BUFFER_H

#include <iostream>

class BitBuffer {
public:
	BitBuffer(std::ostream& out);
	void put(bool b);
};

#endif
