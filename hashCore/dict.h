#pragma once

#include <string>

struct dict {
public:
	std::string word;
	std::string denotation;

	std::string static tolower(const std::string& str) {
		std::string temp;
		for (std::string::size_type i = 0; i < str.length(); i++) {
			if ((str[i] >= 'A') && (str[i] <= 'Z'))
				temp.push_back(str[i] + ('a' - 'A'));
			else temp.push_back(str[i]);
		}

		return temp;
	}

	dict() {
		word = "";
		denotation = "";
	}
	dict(const std::string& _word, const std::string& _denotation) {
		word = _word;
		denotation = _denotation;
	}

	friend bool operator==(const dict& left, const dict& right){
		return (tolower(left.word) == tolower(right.word));
	}
	friend bool operator!=(const dict& left, const dict& right){
		return (tolower(left.word) != tolower(right.word));
	}
	friend bool operator<(const dict& left, const dict& right){
		return (tolower(left.word) < tolower(right.word));
	}
	friend bool operator>(const dict& left, const dict& right){
		return (tolower(left.word) > tolower(right.word));
	}
};
