#pragma once

#include <string>
#include "extList.h"
#include "dict.h"

#define hashSize 49999

class hashCore {
private:
	list<dict> hashTable[hashSize];
	unsigned long hashFunc(const char*);

public:
	bool load(const char*);
	std::string find(const char*);
};