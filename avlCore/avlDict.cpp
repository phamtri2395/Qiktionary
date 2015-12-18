#include "avlDict.h"

#include <fstream>

bool avlCore::load(const char* filename) {
	std::fstream f;
	f.open(filename, std::ios::in);

	if (f) {
		std::string word;
		while (getline(f, word, ':')) {
			std::string denotation;
			getline(f, denotation);

			avlTree.insert(dict(word, denotation));
		}
	}
	else return false;

	f.close();

	return true;
}

std::string avlCore::find(const char* el) {
	dict temp = avlTree.find(dict(el, "<null>"));
	
	return temp.denotation;
}