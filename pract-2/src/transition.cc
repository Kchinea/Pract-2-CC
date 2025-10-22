#include "transition.h"

/**
 * @brief Constructor con vectores (compatibilidad con parser actual).
 * 
 * Convierte los vectores en la estructura de map unificado interno.
 * 
 * @param from Estado origen.
 * @param to Estado destino.
 * @param readSymbols Vector de símbolos leídos (solo usa el primero).
 * @param writeSymbols Vector de símbolos a escribir.
 * @param movements Vector de movimientos.
 */
Transition::Transition(const State& from, const State& to, const std::vector<Symbol>& readSymbols,
                       const std::vector<Symbol>& writeSymbols, const std::vector<Moves>& movements)
  : from(from), to(to), 
    readSymbol(readSymbols.empty() ? Symbol('.') : readSymbols[0]) {
  
  // Convertir vectores a map unificado: cinta -> (símbolo, movimiento)
  size_t maxSize = std::max(writeSymbols.size(), movements.size());
  for (size_t i = 0; i < maxSize; ++i) {
    Symbol writeSymbol = (i < writeSymbols.size()) ? writeSymbols[i] : Symbol('.');
    Moves move = (i < movements.size()) ? movements[i] : Moves::STAY;
    tapeActions.insert(std::make_pair(i, std::make_pair(writeSymbol, move)));
  }
}

/**
 * @brief Constructor moderno con map unificado.
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