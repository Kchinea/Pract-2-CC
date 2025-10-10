#include "state.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const State& state) {
	os << state.id; // Asumiendo que tienes un atributo id
	return os;
}
#include "state.h"
