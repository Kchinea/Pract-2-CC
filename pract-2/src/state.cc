#include "state.h"
#include <iostream>

/**
 * @brief Operador de salida para imprimir un estado.
 * 
 * Imprime el identificador del estado en el stream de salida.
 * 
 * @param os Stream de salida donde se imprimirá el estado.
 * @param state Estado a imprimir.
 * @return Referencia al stream de salida para permitir encadenamiento.
 */
std::ostream& operator<<(std::ostream& os, const State& state) {
	os << state.id; 
	return os;
}
#include "state.h"
