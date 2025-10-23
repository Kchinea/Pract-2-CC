#include "transition.h"

/**
 * @brief Constructor con map unificado.
 * 
 * @param from Estado origen.
 * @param to Estado destino.
 * @param readSymbols Vector de símbolos leídos, uno por cada cinta.
 * @param tapeActions Map de cinta -> (símbolo a escribir, movimiento).
 */
Transition::Transition(const State& from, const State& to, const std::vector<Symbol>& readSymbols,
                       const std::map<int, std::pair<Symbol, Moves>>& tapeActions)
  : from(from), to(to), readSymbols(readSymbols), tapeActions(tapeActions) {
}


/**
 * @brief Operador de salida para imprimir una transición.
 * 
 * Imprime la transición en formato legible mostrando estado origen,
 * estado destino, símbolos de lectura y acciones por cinta.
 * 
 * @param os Stream de salida donde se imprimirá la transición.
 * @param t Transición a imprimir.
 * @return Referencia al stream de salida para permitir encadenamiento.
 */
std::ostream& operator<<(std::ostream& os, const Transition& t){
  os << "Transition(" << t.getFrom() << " -> " << t.getTo() 
     << ", read: [";
  const auto& readSyms = t.getReadSymbols();
  for (size_t i = 0; i < readSyms.size(); ++i) {
    if (i > 0) os << ", ";
    os << readSyms[i];
  }
  os << "], actions: {";
  const auto& actions = t.getTapeActions();
  bool first = true;
  for (const auto& pair : actions) {
    if (!first) os << ", ";
    os << "tape" << pair.first << ":(" << pair.second.first << "," << pair.second.second << ")";
    first = false;
  }
  os << "})";
  return os;
}