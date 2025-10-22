#include "turing_machine_simulator.h"
#include <iostream>

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
  // Get first state from model
  const auto& statesMap = model_.getStates();
  if (!statesMap.empty()) currentState = statesMap.begin()->second;

  bool accepted = false;
  int steps = 0;
  const int MAX_STEPS = 100000;

  while (true) {
    if (trace) {
      os << "Paso " << steps << ": Estado=" << currentState << ", Cintas:\n";
      for (int t = 0; t < tapeCount; ++t) {
        os << "  cinta" << t << ": ";
        for (size_t i = 0; i < tapes[t].size(); ++i) {
          if ((int)i == heads[t]) os << '[';
          os << tapes[t][i];
          if ((int)i == heads[t]) os << ']';
        }
        os << "\n";
      }
    }

    // If current state is accepting, halt and accept
    if (currentState.isAccept()) {
      accepted = true;
      break;
    }

    // Read current symbols and find/apply an applicable transition
    auto currentRead = readCurrentSymbols(tapes, heads);
    const Transition* tr = findApplicableTransition(currentState, tapes, heads);
    if (!tr) break; // no transition
    applyTransition(*tr, tapes, heads, currentState);
    steps++;
    if (steps > MAX_STEPS) break;
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
    if (h >= 0 && h < (int)tapes[t].size()) currentRead[t] = tapes[t][h];
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
 * @return Puntero a la transición aplicable, o nullptr si no hay ninguna.
 */
const Transition* TuringMachineSimulator::findApplicableTransition(
    const State& currentState, 
    const std::vector<std::vector<Symbol>>& tapes, 
    const std::vector<int>& heads) const {
  
  // Leer símbolo de cinta 0
  Symbol currentSymbol = Symbol('.');
  if (!tapes.empty() && !tapes[0].empty() && heads[0] >= 0 && heads[0] < (int)tapes[0].size()) {
    currentSymbol = tapes[0][heads[0]];
  }
  
  // O(1) lookup in model's transition map by state id
  const auto& transitions = model_.getTransitionsFrom(currentState.getId());
  
  // Buscar transición que coincida con el símbolo leído
  for (const auto& tr : transitions) {
    if (tr.getReadSymbol().getValue() == currentSymbol.getValue()) {
      return &tr;
    }
  }
  return nullptr;
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
  
  // Usar el nuevo map unificado tapeActions
  const auto& actions = tr.getTapeActions();
  
  // Aplicar acciones (escribir y mover) por cada cinta definida
  for (const auto& pair : actions) {
    int tapeIndex = pair.first;
    const Symbol& writeSymbol = pair.second.first;
    Moves move = pair.second.second;
    
    // Asegurar que el índice de cinta es válido
    if (tapeIndex >= 0 && tapeIndex < tapeCount) {
      // Escribir símbolo
      tapes[tapeIndex][heads[tapeIndex]] = writeSymbol;
      
      // Mover cabeza
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
  
  // O(1) lookup in model's state map instead of O(n) linear search
  const std::string& toId = tr.getTo().getId();
  const State* nextState = model_.getStateById(toId);
  if (nextState) {
    currentState = *nextState;
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
  if (tapes.empty()) { input = String(std::vector<Symbol>()); return; }
  input = String(tapes[0]);
}
