#pragma once

#include <string>
#include "avlDict.h"
#include "BST.h"
#include "../hashCore/dict.h"

class avlCore {
private:
	AVLtree<dict> avlTree;

public:
	bool load(const char*);
	std::string find(const char*);
};