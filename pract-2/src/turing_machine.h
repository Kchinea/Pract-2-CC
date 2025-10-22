#ifndef TURING_MACHINE_H
#define TURING_MACHINE_H

#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include "string.h"
#include "alphabet.h"
#include "state.h"
#include "transition.h"
#include "turing_machine_model.h"
#include "turing_machine_simulator.h"

/**
 * @brief Wrapper de Máquina de Turing que delega a Model y Simulator.
 * 
 * Mantiene compatibilidad hacia atrás con el código existente mientras
 * internamente usa la arquitectura separada de Model (datos) y Simulator (lógica).
 * Este patrón Facade simplifica el uso externo mientras permite extensibilidad.
 */
class TuringMachine {
 public:
  TuringMachine() = default;
  
  /**
   * @brief Constructor que crea el modelo y el simulador internamente.
   * 
   * @param states Vector de estados obtenidos del parser.
   * @param transitions Vector de transiciones obtenidas del parser.
   * @param stringAlphabet Alfabeto de entrada de la MT.
   * @param tapeAlphabet Alfabeto de cinta de la MT.
   */
  TuringMachine(std::vector<State> states, std::vector<Transition> transitions, 
                Alphabet stringAlphabet, Alphabet tapeAlphabet);
  
  // Getters que delegan al modelo
  const std::unordered_map<std::string, State>& getStates() const;
  const std::map<std::string, std::vector<Transition>>& getTransitions() const;
  const Alphabet& getStringAlphabet() const;
  const Alphabet& getTapeAlphabet() const;
  
  // Acceso directo al modelo y simulador para uso avanzado
  const TuringMachineModel& getModel() const { return *model_; }
  const TuringMachineSimulator& getSimulator() const { return *simulator_; }
  
  friend std::ostream& operator<<(std::ostream& os, const TuringMachine& tm);
  
  /**
   * @brief Simula la ejecución delegando al simulador.
   * 
   * @param input Cadena de entrada (se modifica para contener resultado).
   * @param trace Si es true, imprime el trazo de ejecución.
   * @param os Stream de salida para el trazo.
   * @return true si la cadena es aceptada, false en caso contrario.
   */
  bool compute(String& input, bool trace, std::ostream& os) const;
  
 private:
  std::unique_ptr<TuringMachineModel> model_;
  std::unique_ptr<TuringMachineSimulator> simulator_;
};

#endif