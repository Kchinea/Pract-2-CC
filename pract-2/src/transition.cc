#include "transition.h"

/**
 * @brief Constructor con map unificado.
 * 
 * @param from Estado origen.
 * @param to Estado destino.
 * @param readSymbol Símbolo leído de la cinta 0.
 * @param tapeActions Map de cinta -> (símbolo a escribir, movimiento).
 */
Transition::Transition(const State& from, const State& to, const Symbol& readSymbol,
                       const std::map<int, std::pair<Symbol, Moves>>& tapeActions)
  : from(from), to(to), readSymbol(readSymbol), tapeActions(tapeActions) {
}


/**
 * @brief Operador de salida para imprimir una transición.
 * 
 * Imprime la transición en formato legible mostrando estado origen,
 * estado destino, símbolo de lectura y acciones por cinta.
 * 
 * @param os Stream de salida donde se imprimirá la transición.
 * @param t Transición a imprimir.
 * @return Referencia al stream de salida para permitir encadenamiento.
 */
std::ostream& operator<<(std::ostream& os, const Transition& t){
  os << "Transition(" << t.getFrom() << " -> " << t.getTo() 
     << ", read: " << t.getReadSymbol() << ", actions: {";
  
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