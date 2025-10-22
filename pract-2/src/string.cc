#include "string.h"

/**
 * @brief Operador de salida para imprimir una cadena de símbolos.
 * 
 * Imprime cada símbolo de la cadena secuencialmente en el stream de salida.
 * 
 * @param os Stream de salida donde se imprimirá la cadena.
 * @param string Cadena a imprimir.
 * @return Referencia al stream de salida para permitir encadenamiento.
 */
std::ostream& operator<<(std::ostream& os, const String& string) {
  for (const auto& symbol : string.symbols) os << symbol;
  return os;
}