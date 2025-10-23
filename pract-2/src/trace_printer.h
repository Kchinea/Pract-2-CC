#ifndef TRACE_PRINTER_H
#define TRACE_PRINTER_H

#include <ostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>
#include "state.h"
#include "symbol.h"
#include "transition.h"
#include "moves.h"

/**
 * @brief Clase responsable de imprimir la traza de ejecución de la Máquina de Turing.
 * 
 * Se encarga exclusivamente del formateo y presentación de la información de traza,
 * separando esta responsabilidad del simulador. Sigue el principio de Single Responsibility.
 */
class TracePrinter {
 public:
  void printHeader(std::ostream& os) const;
  void printStep(std::ostream& os, int step, 
                const State& currentState,
                const std::vector<Symbol>& currentRead, 
                const Transition& tr, bool foundTransition,
                const std::vector<std::vector<Symbol>>& tapes,
                const std::vector<int>& heads, int tapeCount) const;
  void printAcceptedMessage(std::ostream& os) const;
  void printRejectedMessage(std::ostream& os) const;
  void printMaxStepsMessage(std::ostream& os) const;
};

#endif
