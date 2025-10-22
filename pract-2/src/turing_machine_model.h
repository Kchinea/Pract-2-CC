#ifndef TURING_MACHINE_MODEL_H
#define TURING_MACHINE_MODEL_H

#include <vector>
#include <unordered_map>
#include <map>
#include "alphabet.h"
#include "state.h"
#include "transition.h"

/**
 * @brief Modelo de una Máquina de Turing multicinta.
 * 
 * Representa la definición formal de una MT: conjunto de estados, transiciones
 * y alfabetos. Se encarga únicamente de almacenar y proporcionar acceso a la
 * estructura de la máquina, sin responsabilidad sobre la simulación.
 */
class TuringMachineModel {
 public:
  TuringMachineModel() = default;
  TuringMachineModel(std::vector<State> states, std::vector<Transition> transitions, 
                     Alphabet stringAlphabet, Alphabet tapeAlphabet, const std::string& initialStateId);
  const std::unordered_map<std::string, State>& getStates() const { return statesMap_; }
  const std::map<std::string, std::vector<Transition>>& getTransitions() const { return transitionsMap_; }
  const Alphabet& getStringAlphabet() const { return stringAlphabet_; }
  const Alphabet& getTapeAlphabet() const { return tapeAlphabet_; }
  const State& getInitialState() const;
  const State& getStateById(const std::string& id) const;
  const std::vector<Transition>& getTransitionsFrom(const std::string& stateId) const;
  int determineTapeCount() const;
  friend std::ostream& operator<<(std::ostream& os, const TuringMachineModel& model);
 private:
  std::unordered_map<std::string, State> statesMap_;
  std::map<std::string, std::vector<Transition>> transitionsMap_;
  Alphabet stringAlphabet_;
  Alphabet tapeAlphabet_;
  std::string initialStateId_;
  static const std::vector<Transition> emptyTransitions_; 
};

#endif
