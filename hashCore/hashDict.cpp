#include "hashDict.h"

#include <fstream>

unsigned long hashCore::hashFunc(const char* str) {
	int M = 33;
	int init_value = 5381;
	unsigned long hash = init_value;

	int c = 0;
	while (str[c] != '\0') {
		if ((str[c] >= 'A') && (str[c] <= 'Z'))
			hash = hash * M + (str[c] + ('a' - 'A'));
		else
			hash = hash * M + (str[c]);

		c++;
	}

	return (hash % hashSize);
}

bool hashCore::load(const char* filename) {
	std::fstream f;
	f.open(filename, std::ios::in);

	if (f) {
		std::string word;
		std::string denotation;
		while (getline(f, word, ':')) {
			getline(f, denotation);
			hashTable[hashFunc(word.c_str())].push(dict(word, denotation));
		}
	}
	else return false;

	f.close();

	return true;
}

std::string hashCore::find(const char* el) {
	dict temp = hashTable[hashFunc(el)].find(dict(el,"<null>"));

	return (temp.denotation);
}


