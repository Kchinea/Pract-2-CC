#include "turing_machine.h"
#include <iostream> 

/**
 * @brief Constructor que crea el modelo y el simulador internamente.
 * 
 * Delega la construcción del modelo a TuringMachineModel y luego crea
 * un simulador que trabaja sobre ese modelo. Mantiene compatibilidad
 * hacia atrás mientras usa la arquitectura separada.
 * 
 * @param states Vector de estados obtenidos del parser.
 * @param transitions Vector de transiciones obtenidas del parser.
 * @param stringAlphabet Alfabeto de entrada de la MT.
 * @param tapeAlphabet Alfabeto de cinta de la MT.
 * @param initialStateId Identificador del estado inicial.
 */
TuringMachine::TuringMachine(std::vector<State> states, std::vector<Transition> transitions, 
                             Alphabet stringAlphabet, Alphabet tapeAlphabet, const std::string& initialStateId) 
  : model_(states, transitions, stringAlphabet, tapeAlphabet, initialStateId),
    simulator_(model_) {
}

/**
 * @brief Simula la ejecución delegando al simulador.
 * 
 * @param input Cadena de entrada (se modifica para contener resultado).
 * @param trace Si es true, imprime el trazo de ejecución.
 * @param os Stream de salida para el trazo.
 * @return true si la cadena es aceptada, false en caso contrario.
 */
bool TuringMachine::compute(String& input, bool trace, std::ostream& os) const {
  return simulator_.compute(input, trace, os);
}

// Getters que delegan al modelo
const std::unordered_map<std::string, State>& TuringMachine::getStates() const {
  return model_.getStates();
}

const std::map<std::string, std::vector<Transition>>& TuringMachine::getTransitions() const {
  return model_.getTransitions();
}

const Alphabet& TuringMachine::getStringAlphabet() const {
  return model_.getStringAlphabet();
}

const Alphabet& TuringMachine::getTapeAlphabet() const {
  return model_.getTapeAlphabet();
}

/**
 * @brief Operador de salida que delega al modelo.
 * 
 * @param os Stream de salida donde se imprimirá la máquina.
 * @param tm Máquina de Turing a imprimir.
 * @return Referencia al stream de salida para permitir encadenamiento.
 */
std::ostream& operator<<(std::ostream& os, const TuringMachine& tm) {
  os << tm.model_;
  return os;
}