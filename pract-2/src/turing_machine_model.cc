#include "turing_machine_model.h"
#include <iostream>
#include <stdexcept>

// Inicializar vector estático vacío para transiciones
const std::vector<Transition> TuringMachineModel::emptyTransitions_;

/**
 * @brief Constructor que construye el modelo a partir de vectores.
 * 
 * Toma los vectores de estados y transiciones del parser y los convierte en estructuras
 * de datos optimizadas: un unordered_map para acceso O(1) a estados por id, y un map
 * que indexa las transiciones por estado origen para búsqueda eficiente.
 * 
 * @param states Vector de estados obtenidos del parser.
 * @param transitions Vector de transiciones obtenidas del parser.
 * @param stringAlphabet Alfabeto de entrada de la MT.
 * @param tapeAlphabet Alfabeto de cinta de la MT.
 * @param initialStateId Identificador del estado inicial.
 */
TuringMachineModel::TuringMachineModel(std::vector<State> states, std::vector<Transition> transitions, 
                                       Alphabet stringAlphabet, Alphabet tapeAlphabet, const std::string& initialStateId)
  : stringAlphabet_(stringAlphabet), tapeAlphabet_(tapeAlphabet), initialStateId_(initialStateId) {
  for (const auto& state : states) {
    statesMap_[state.getId()] = state;
  }
  for (const auto& transition : transitions) {
    transitionsMap_[transition.getFrom().getId()].push_back(transition);
  }
}

/**
 * @brief Obtiene el estado inicial de la máquina.
 * 
 * @return Referencia al estado inicial.
 * @throws std::runtime_error si el estado inicial no existe.
 */
const State& TuringMachineModel::getInitialState() const {
  return getStateById(initialStateId_);
}

/**
 * @brief Busca un estado por su identificador.
 * 
 * @param id Identificador del estado.
 * @return Referencia al estado.
 * @throws std::runtime_error si el estado no existe.
 */
const State& TuringMachineModel::getStateById(const std::string& id) const {
  auto it = statesMap_.find(id);
  if (it != statesMap_.end()) {
    return it->second;
  }
  throw std::runtime_error("Estado no encontrado: " + id);
}

/**
 * @brief Obtiene las transiciones aplicables desde un estado dado.
 * 
 * @param stateId Identificador del estado origen.
 * @return Vector de transiciones desde ese estado (vacío si no hay ninguna).
 */
const std::vector<Transition>& TuringMachineModel::getTransitionsFrom(const std::string& stateId) const {
  auto it = transitionsMap_.find(stateId);
  if (it != transitionsMap_.end()) {
    return it->second;
  }
  return emptyTransitions_;
}

/**
 * @brief Determina el número de cintas necesarias según las transiciones.
 * 
 * Analiza todas las transiciones y toma el número de cinta más alto usado
 * en las acciones (tapeActions) para determinar cuántas cintas necesita la máquina.
 * 
 * @return Número de cintas que debe usar la simulación.
 */
int TuringMachineModel::determineTapeCount() const {
  int tapeCount = 1;
  for (const auto& statePair : transitionsMap_) {
    for (const auto& transition : statePair.second) {
      const auto& actions = transition.getTapeActions();
      for (const auto& actionPair : actions) {
        tapeCount = std::max(tapeCount, actionPair.first + 1);
      }
    }
  }
  return tapeCount;
}

/**
 * @brief Operador de salida para imprimir el modelo de una máquina de Turing.
 * 
 * Imprime todos los componentes del modelo: estados (con indicación de cuáles
 * son de aceptación), alfabeto de entrada, alfabeto de cinta y todas las transiciones.
 * 
 * @param os Stream de salida donde se imprimirá el modelo.
 * @param model Modelo de Máquina de Turing a imprimir.
 * @return Referencia al stream de salida para permitir encadenamiento.
 */
std::ostream& operator<<(std::ostream& os, const TuringMachineModel& model) {
  os << "States:\n";
  for (const auto& statePair : model.statesMap_) {
    const State& state = statePair.second;
    os << state;
    if (state.isAccept()) os << " (accept)";
    os << std::endl;
  }
  os << "String Alphabet: " << model.stringAlphabet_ << std::endl;
  os << "Tape Alphabet: " << model.tapeAlphabet_ << std::endl;
  os << "Transitions:" << std::endl;
  for (const auto& transPair : model.transitionsMap_) {
    for (const auto& transition : transPair.second) {
      os << transition << std::endl;
    }
  }
  return os;
}
