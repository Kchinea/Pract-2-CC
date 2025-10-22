#include "alphabet.h"
#include <iostream>

/**
 * @brief Operador de salida para imprimir un alfabeto.
 * 
 * Imprime cada símbolo del alfabeto separado por espacios en el stream de salida.
 * 
 * @param os Stream de salida donde se imprimirá el alfabeto.
 * @param alphabet Alfabeto a imprimir.
 * @return Referencia al stream de salida para permitir encadenamiento.
 */
std::ostream& operator<<(std::ostream& os, const Alphabet& alphabet) {
	for (const auto& symbol : alphabet.symbols)
	  os << symbol << " ";
	return os;
}
#include "alphabet.h"
