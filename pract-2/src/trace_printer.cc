#include "trace_printer.h"

/**
 * @brief Imprime el encabezado de la traza de ejecución.
 * 
 * @param os Stream de salida donde se imprimirá el encabezado.
 */
void TracePrinter::printHeader(std::ostream& os) const {
  os << "\n" << std::string(120, '=') << "\n";
  os << "TRAZA DE EJECUCIÓN\n";
  os << std::string(120, '=') << "\n\n";
}

/**
 * @brief Imprime la información de un paso de la ejecución.
 * 
 * Muestra el número de paso, el estado actual, los símbolos leídos de cada cinta,
 * la transición aplicada (si existe), las acciones que se realizarán y el estado
 * actual de todas las cintas con la posición de los cabezales.
 * 
 * @param os Stream de salida donde se imprimirá el paso.
 * @param step Número del paso actual.
 * @param currentState Estado actual de la máquina.
 * @param currentRead Vector con los símbolos leídos de cada cinta.
 * @param tr Transición que se aplicará (o emptyTransition_ si no hay).
 * @param foundTransition Indica si se encontró una transición aplicable.
 * @param tapes Vector de cintas en su estado actual.
 * @param heads Vector con las posiciones de los cabezales.
 * @param tapeCount Número total de cintas.
 */
void TracePrinter::printStep(std::ostream& os, int step, 
                             const State& currentState,
                             const std::vector<Symbol>& currentRead, 
                             const Transition& tr, bool foundTransition,
                             const std::vector<std::vector<Symbol>>& tapes,
                             const std::vector<int>& heads, int tapeCount) const {
  // Print step header
  os << "╔═══════════════════════════════════════════════════════════════════════════════════════════════╗\n";
  os << "║  PASO " << std::setw(3) << std::left << step << std::string(85, ' ') << "║\n";
  os << "╠═══════════════════════════════════════════════════════════════════════════════════════════════╣\n";
  
  // Current state
  os << "║  Estado actual: " << std::setw(73) << std::left << currentState.getId() << "║\n";
  
  // Read symbols
  os << "║  Símbolos leídos: ";
  std::ostringstream readStr;
  for (int t = 0; t < tapeCount; ++t) {
    if (t > 0) readStr << ", ";
    readStr << "cinta" << t << "=[" << currentRead[t] << "]";
  }
  os << std::setw(71) << std::left << readStr.str() << "║\n";
  
  // Transition info
  if (foundTransition) {
    os << "║  Transición: " << std::setw(76) << std::left 
       << (currentState.getId() + " → " + tr.getTo().getId()) << "║\n";
    
    // Actions for each tape
    os << "║  Acciones:";
    for (int t = 0; t < tapeCount; ++t) {
      const auto& actions = tr.getTapeActions();
      auto it = actions.find(t);
      if (it != actions.end()) {
        std::ostringstream actionStr;
        actionStr << "    cinta" << t << ": escribir '" << it->second.first << "', mover ";
        switch(it->second.second) {
          case Moves::LEFT: actionStr << "←(izq)"; break;
          case Moves::RIGHT: actionStr << "→(der)"; break;
          case Moves::STAY: actionStr << "•(quieto)"; break;
        }
        if (t == 0) {
          os << std::setw(79) << std::left << actionStr.str() << "║\n";
        } else {
          os << "║              " << std::setw(75) << std::left << actionStr.str() << "║\n";
        }
      }
    }
  } else {
    os << "║  Transición: " << std::setw(76) << std::left 
       << "NO HAY TRANSICIÓN APLICABLE" << "║\n";
  }
  
  // Tape contents with head position
  os << "╠═══════════════════════════════════════════════════════════════════════════════════════════════╣\n";
  os << "║  Estado de las cintas:";
  for (int t = 0; t < tapeCount; ++t) {
    std::ostringstream tapeStr;
    tapeStr << "    cinta" << t << ": ";
    for (size_t i = 0; i < tapes[t].size(); ++i) {
      if ((int)i == heads[t]) tapeStr << '[';
      tapeStr << tapes[t][i];
      if ((int)i == heads[t]) tapeStr << ']';
      tapeStr << ' ';
    }
    if (t == 0) {
      os << std::setw(67) << std::left << tapeStr.str() << "║\n";
    } else {
      os << "║                        " << std::setw(65) << std::left << tapeStr.str() << "║\n";
    }
  }
  os << "╚═══════════════════════════════════════════════════════════════════════════════════════════════╝\n\n";
}

/**
 * @brief Imprime el mensaje cuando se alcanza un estado de aceptación.
 * 
 * @param os Stream de salida donde se imprimirá el mensaje.
 */
void TracePrinter::printAcceptedMessage(std::ostream& os) const {
  os << "\n┌─────────────────────────────────────────────────────────────┐\n";
  os << "│  ✓ ESTADO DE ACEPTACIÓN ALCANZADO - CADENA ACEPTADA         │\n";
  os << "└─────────────────────────────────────────────────────────────┘\n";
}

/**
 * @brief Imprime el mensaje cuando no se encuentra transición aplicable.
 * 
 * @param os Stream de salida donde se imprimirá el mensaje.
 */
void TracePrinter::printRejectedMessage(std::ostream& os) const {
  os << "\n┌─────────────────────────────────────────────────────────────┐\n";
  os << "│  ✗ NO HAY TRANSICIÓN - CADENA RECHAZADA                     │\n";
  os << "└─────────────────────────────────────────────────────────────┘\n";
}

/**
 * @brief Imprime el mensaje cuando se excede el límite de pasos.
 * 
 * @param os Stream de salida donde se imprimirá el mensaje.
 */
void TracePrinter::printMaxStepsMessage(std::ostream& os) const {
  os << "\n┌─────────────────────────────────────────────────────────────┐\n";
  os << "│  ⚠ LÍMITE DE PASOS EXCEDIDO - EJECUCIÓN DETENIDA            │\n";
  os << "└─────────────────────────────────────────────────────────────┘\n";
}
