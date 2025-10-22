#ifndef TURING_MACHINE_SIMULATOR_H
#define TURING_MACHINE_SIMULATOR_H

#include <vector>
#include <ostream>
#include "turing_machine_model.h"
#include "string.h"
#include "symbol.h"
#include "transition.h"

/**
 * @brief Simulador de una Máquina de Turing multicinta.
 * 
 * Se encarga de ejecutar la simulación de una MT sobre cadenas de entrada.
 * Mantiene el estado de las cintas, cabezas y estado actual durante la ejecución.
 * Separado del modelo para seguir el principio de Single Responsibility.
 */
class TuringMachineSimulator {
 public:
  /**
   * @brief Constructor del simulador.
   * 
   * @param model Referencia al modelo de la MT que se va a simular.
   */
  explicit TuringMachineSimulator(const TuringMachineModel& model);
  
  /**
   * @brief Simula la ejecución de la MT sobre una cadena de entrada.
   * 
   * @param input Cadena de entrada (se modifica para contener la cinta 0 final).
   * @param trace Si es true, imprime el trazo de ejecución.
   * @param os Stream de salida para el trazo.
   * @return true si la cadena es aceptada, false en caso contrario.
   */
  bool compute(String& input, bool trace, std::ostream& os) const;

 private:
  // Helper methods para la simulación
  void initializeTapes(const String& input, int tapeCount, 
                      std::vector<std::vector<Symbol>>& tapes, 
                      std::vector<int>& heads) const;
  
  std::vector<Symbol> readCurrentSymbols(const std::vector<std::vector<Symbol>>& tapes, 
                                         const std::vector<int>& heads) const;
  
  const Transition* findApplicableTransition(const State& currentState, 
                                             const std::vector<std::vector<Symbol>>& tapes, 
                                             const std::vector<int>& heads) const;
  
  void applyTransition(const Transition& tr, 
                      std::vector<std::vector<Symbol>>& tapes, 
                      std::vector<int>& heads, 
                      State& currentState) const;
  
  void flattenResult(String& input, const std::vector<std::vector<Symbol>>& tapes) const;

 private:
  const TuringMachineModel& model_;  // Referencia al modelo (no ownership)
};

#endif
