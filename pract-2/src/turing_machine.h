#ifndef TURING_MACHINE_H
#define TURING_MACHINE_H

#include <vector>
#include <unordered_map>
#include <map>
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
  TuringMachine(std::vector<State> states, std::vector<Transition> transitions, 
                Alphabet stringAlphabet, Alphabet tapeAlphabet, const std::string& initialStateId);
  const std::unordered_map<std::string, State>& getStates() const;
  const std::map<std::string, std::vector<Transition>>& getTransitions() const;
  const Alphabet& getStringAlphabet() const;
  const Alphabet& getTapeAlphabet() const;
  const TuringMachineModel& getModel() const { return model_; }
  const TuringMachineSimulator& getSimulator() const { return simulator_; }
  friend std::ostream& operator<<(std::ostream& os, const TuringMachine& tm);
  bool compute(String& input, bool trace, std::ostream& os) const;
 private:
  TuringMachineModel model_;
  TuringMachineSimulator simulator_;
};

#endif