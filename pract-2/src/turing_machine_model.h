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
  
  /**
   * @brief Constructor que construye el modelo a partir de vectores.
   * 
   * @param states Vector de estados obtenidos del parser.
   * @param transitions Vector de transiciones obtenidas del parser.
   * @param stringAlphabet Alfabeto de entrada de la MT.
   * @param tapeAlphabet Alfabeto de cinta de la MT.
   */
  TuringMachineModel(std::vector<State> states, std::vector<Transition> transitions, 
                     Alphabet stringAlphabet, Alphabet tapeAlphabet);

  // Getters
  const std::unordered_map<std::string, State>& getStates() const { return statesMap_; }
  const std::map<std::string, std::vector<Transition>>& getTransitions() const { return transitionsMap_; }
  const Alphabet& getStringAlphabet() const { return stringAlphabet_; }
  const Alphabet& getTapeAlphabet() const { return tapeAlphabet_; }
  
  /**
   * @brief Busca un estado por su identificador.
   * 
   * @param id Identificador del estado.
   * @return Puntero al estado si existe, nullptr en caso contrario.
   */
  const State* getStateById(const std::string& id) const;
  
  /**
   * @brief Obtiene las transiciones aplicables desde un estado dado.
   * 
   * @param stateId Identificador del estado origen.
   * @return Vector de transiciones desde ese estado (vacío si no hay ninguna).
   */
  const std::vector<Transition>& getTransitionsFrom(const std::string& stateId) const;
  
  /**
   * @brief Determina el número de cintas necesarias según las transiciones.
   * 
   * @return Número de cintas que debe usar la simulación.
   */
  int determineTapeCount() const;
  
  friend std::ostream& operator<<(std::ostream& os, const TuringMachineModel& model);

 private:
  std::unordered_map<std::string, State> statesMap_;  // O(1) access by state id
  std::map<std::string, std::vector<Transition>> transitionsMap_;  // transitions indexed by source state id
  Alphabet stringAlphabet_;
  Alphabet tapeAlphabet_;
  
  static const std::vector<Transition> emptyTransitions_;  // Para devolver cuando no hay transiciones
};

#endif
