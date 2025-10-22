#include "turing_machine_simulator.h"
#include <stdexcept>

// Inicializar transición estática vacía
const Transition TuringMachineSimulator::emptyTransition_(
  State(""), State(""), Symbol('.'), std::map<int, std::pair<Symbol, Moves>>()
);

/**
 * @brief Constructor del simulador.
 * 
 * @param model Referencia al modelo de la MT que se va a simular.
 */
TuringMachineSimulator::TuringMachineSimulator(const TuringMachineModel& model)
  : model_(model) {
}

/**
 * @brief Simula la ejecución de la máquina de Turing multicinta sobre una cadena de entrada.
 * 
 * Inicializa las cintas con la cadena de entrada en la cinta 0, determina el número de cintas
 * necesarias según las transiciones, y ejecuta la simulación paso a paso. Acepta si alcanza
 * un estado de aceptación, rechaza si no encuentra transición aplicable o si supera el límite
 * de pasos. Soporta modo traza para debugging.
 * 
 * @param input Cadena de entrada (se modifica para contener la cinta 0 final al terminar).
 * @param trace Si es true, imprime el trazo de ejecución en el stream os.
 * @param os Stream de salida donde se imprimirá el trazo (si trace es true).
 * @return true si la cadena es aceptada, false en caso contrario.
 */
bool TuringMachineSimulator::compute(String& input, bool trace, std::ostream& os) const {
  int tapeCount = model_.determineTapeCount();
  std::vector<std::vector<Symbol>> tapes;
  std::vector<int> heads;
  initializeTapes(input, tapeCount, tapes, heads);
  State currentState;
  try {
    currentState = model_.getInitialState();
  } catch (const std::exception& e) {
    throw std::runtime_error("No se pudo obtener el estado inicial de la máquina: " + std::string(e.what()));
  }
  bool accepted = false;
  int steps = 0;
  const int MAX_STEPS = 100000;
  if (trace) tracePrinter_.printHeader(os);
  while (true) {
    if (currentState.isAccept()) {
      accepted = true;
      if (trace) tracePrinter_.printAcceptedMessage(os);
      break;
    }
    auto currentRead = readCurrentSymbols(tapes, heads);
    bool foundTransition = false;
    const Transition& tr = findApplicableTransition(currentState, tapes, heads, foundTransition);
    if (trace) {
      tracePrinter_.printStep(os, steps, currentState, currentRead, tr, foundTransition, tapes, heads, tapeCount);
    }
    if (!foundTransition) {
      if (trace) tracePrinter_.printRejectedMessage(os);
      break;
    }
    applyTransition(tr, tapes, heads, currentState);
    steps++;
    if (steps > MAX_STEPS) {
      if (trace) tracePrinter_.printMaxStepsMessage(os);
      break;
    }
  }
  flattenResult(input, tapes);
  return accepted;
}

/**
 * @brief Inicializa las cintas y las cabezas para la simulación.
 * 
 * Crea el número especificado de cintas, coloca la cadena de entrada en la cinta 0,
 * e inicializa las demás cintas con un símbolo blanco. Todas las cabezas comienzan
 * en la posición 0.
 * 
 * @param input Cadena de entrada para colocar en la cinta 0.
 * @param tapeCount Número de cintas a crear.
 * @param tapes Vector de cintas (salida, se modificará).
 * @param heads Vector de posiciones de cabezas (salida, se modificará).
 */
void TuringMachineSimulator::initializeTapes(const String& input, int tapeCount, 
                                             std::vector<std::vector<Symbol>>& tapes, 
                                             std::vector<int>& heads) const {
  tapes.assign(tapeCount, std::vector<Symbol>());
  heads.assign(tapeCount, 0);
  for (const auto& s : input.getSymbols()) tapes[0].push_back(s);
  if (tapes[0].empty()) tapes[0].push_back(Symbol('.'));
  for (int i = 1; i < tapeCount; ++i) tapes[i].push_back(Symbol('.'));
}

/**
 * @brief Lee los símbolos actuales bajo cada cabeza de lectura.
 * 
 * Para cada cinta, lee el símbolo en la posición indicada por la cabeza.
 * Si la cabeza está fuera de los límites de la cinta, devuelve el símbolo blanco '.'.
 * 
 * @param tapes Vector de cintas.
 * @param heads Vector de posiciones de cabezas.
 * @return Vector de símbolos leídos, uno por cinta.
 */
std::vector<Symbol> TuringMachineSimulator::readCurrentSymbols(
    const std::vector<std::vector<Symbol>>& tapes, 
    const std::vector<int>& heads) const {
  int tapeCount = tapes.size();
  std::vector<Symbol> currentRead(tapeCount, Symbol('.'));
  for (int t = 0; t < tapeCount; ++t) {
    int h = heads[t];
    if (h >= 0 && h < (int)tapes[t].size()){
      currentRead[t] = tapes[t][h];
    }
  }
  return currentRead;
}

/**
 * @brief Busca una transición aplicable al estado actual y símbolos leídos.
 * 
 * Usa el map de transiciones del modelo para buscar directamente las transiciones 
 * que parten del estado actual (O(1) en lugar de O(n)). Solo compara el símbolo
 * leído de la cinta 0 con readSymbol de cada transición.
 * 
 * @param currentState Estado actual de la máquina.
 * @param tapes Vector de cintas.
 * @param heads Vector de posiciones de cabezas.
 * @param found Referencia bool que se establece a true si se encuentra una transición.
 * @return Referencia a la transición encontrada, o a emptyTransition_ si no se encuentra.
 */
const Transition& TuringMachineSimulator::findApplicableTransition(
    const State& currentState, 
    const std::vector<std::vector<Symbol>>& tapes, 
    const std::vector<int>& heads,
    bool& found) const {
  Symbol currentSymbol = Symbol('.');
  if (!tapes.empty() && !tapes[0].empty() && heads[0] >= 0 && heads[0] < (int)tapes[0].size()) {
    currentSymbol = tapes[0][heads[0]];
  }
  const auto& transitions = model_.getTransitionsFrom(currentState.getId());
  for (const auto& transition : transitions) {
    if (transition.getReadSymbol().getValue() == currentSymbol.getValue()) {
      found = true;
      return transition;
    }
  }
  found = false;
  return emptyTransition_;
}

/**
 * @brief Aplica una transición: escribe símbolos, mueve cabezas y cambia de estado.
 * 
 * Escribe los símbolos especificados en cada cinta bajo las cabezas, mueve cada
 * cabeza según el vector de movimientos (LEFT, RIGHT o STAY), expandiendo las
 * cintas si es necesario, y actualiza el estado actual usando el map de estados
 * del modelo para búsqueda O(1) (para preservar flags de aceptación).
 * 
 * @param tr Transición a aplicar.
 * @param tapes Vector de cintas (se modificará).
 * @param heads Vector de posiciones de cabezas (se modificará).
 * @param currentState Estado actual (se modificará al estado destino).
 */
void TuringMachineSimulator::applyTransition(const Transition& tr, 
                                             std::vector<std::vector<Symbol>>& tapes, 
                                             std::vector<int>& heads, 
                                             State& currentState) const {
  int tapeCount = tapes.size();
  const std::map<int, std::pair<Symbol, Moves>>& actions = tr.getTapeActions();
  for (const auto& pair : actions) {
    int tapeIndex = pair.first;
    const Symbol& writeSymbol = pair.second.first;
    Moves move = pair.second.second;
    if (tapeIndex >= 0 && tapeIndex < tapeCount) {
      tapes[tapeIndex][heads[tapeIndex]] = writeSymbol;
      if (move == Moves::LEFT) {
        if (heads[tapeIndex] == 0) {
          tapes[tapeIndex].insert(tapes[tapeIndex].begin(), Symbol('.'));
        } else {
          heads[tapeIndex]--;
        }
      } else if (move == Moves::RIGHT) {
        heads[tapeIndex]++;
        if (heads[tapeIndex] == (int)tapes[tapeIndex].size()) {
          tapes[tapeIndex].push_back(Symbol('.'));
        }
      }
      // Si es STAY, no hacer nada
    }
  }
  const std::string& toId = tr.getTo().getId();
  try {
    currentState = model_.getStateById(toId);
  } catch (const std::exception& e) {
    throw std::runtime_error("Error aplicando transición: estado destino no encontrado: " + toId);
  }
}

/**
 * @brief Aplana el resultado de la simulación en la cadena de salida.
 * 
 * Toma el contenido de la cinta 0 tras la ejecución y lo almacena en el
 * objeto String de entrada para que el llamador pueda acceder al resultado.
 * 
 * @param input String donde se guardará el resultado (se modificará).
 * @param tapes Vector de cintas con el estado final.
 */
void TuringMachineSimulator::flattenResult(String& input, 
                                           const std::vector<std::vector<Symbol>>& tapes) const {
  if (tapes.empty()) { 
    input = String(std::vector<Symbol>()); 
    return; 
  }
  input = String(tapes[0]);
}
