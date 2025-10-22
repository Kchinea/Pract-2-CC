#include "symbol.h"
#include <iostream>

/**
 * @brief Operador de salida para imprimir un símbolo.
 * 
 * Imprime el valor del carácter del símbolo en el stream de salida.
 * 
 * @param os Stream de salida donde se imprimirá el símbolo.
 * @param symbol Símbolo a imprimir.
 * @return Referencia al stream de salida para permitir encadenamiento.
 */
std::ostream& operator<<(std::ostream& os, const Symbol& symbol) {
	os << symbol.getValue(); 
	return os;
}
#include "symbol.h"
