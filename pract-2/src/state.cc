#include "state.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const State& state) {
	os << state.id; 
	return os;
}
#include "state.h"
