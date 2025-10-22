#ifndef TURING_MACHINE_SIMULATOR_H
#define TURING_MACHINE_SIMULATOR_H

#include <vector>
#include <ostream>
#include "turing_machine_model.h"
#include "trace_printer.h"
#include "string.h"
#include "symbol.h"
#include "transition.h"

/**
 * @brief Simulador de una Máquina de Turing multicinta.
 * 
 * Se encarga de ejecutar la simulación de una MT sobre cadenas de entrada.
 * Mantiene el estado de las cintas, cabezas y estado actual durante la ejecución.
 * Usa composición con TracePrinter para delegar la responsabilidad de impresión.
 */
class TuringMachineSimulator {
 public:
  explicit TuringMachineSimulator(const TuringMachineModel& model);
  bool compute(String& input, bool trace, std::ostream& os) const;
 private:
  void initializeTapes(const String& input, int tapeCount, std::vector<std::vector<Symbol>>& tapes, 
                      std::vector<int>& heads) const;
  std::vector<Symbol> readCurrentSymbols(const std::vector<std::vector<Symbol>>& tapes, 
                                         const std::vector<int>& heads) const;
  const Transition& findApplicableTransition(const State& currentState, 
                                             const std::vector<std::vector<Symbol>>& tapes, 
                                             const std::vector<int>& heads,
                                             bool& found) const;
  void applyTransition(const Transition& tr, 
                      std::vector<std::vector<Symbol>>& tapes, 
                      std::vector<int>& heads, 
                      State& currentState) const;
  void flattenResult(String& input, const std::vector<std::vector<Symbol>>& tapes) const;
  const TuringMachineModel& model_;
  TracePrinter tracePrinter_;
  static const Transition emptyTransition_;
};

#endif
