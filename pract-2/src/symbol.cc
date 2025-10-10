#include "symbol.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Symbol& symbol) {
	os << symbol.getValue(); // Asumiendo que tienes un atributo value
	return os;
}
#include "symbol.h"
