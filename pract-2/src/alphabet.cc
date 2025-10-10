#include "alphabet.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Alphabet& alphabet) {
	for (const auto& symbol : alphabet.symbols)
	  os << symbol << " ";
	return os;
}
#include "alphabet.h"
