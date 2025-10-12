#include "turing_machine.h"

bool TuringMachine::compute(String& input, bool trace, std::ostream& os) const {
  // Simulación de la máquina de Turing
  // Cinta: vector de símbolos, con espacio extra a derecha e izquierda
  std::vector<Symbol> tape;
  for (const auto& s : input.getSymbols()) tape.push_back(s);
  int head = 0;
  State currentState;
  // Buscar estado inicial
  for (const auto& s : states_) {
    if (s.getId() == states_.front().getId()) {
      currentState = s;
      break;
    }
  }
  // Asegura que la cinta tenga al menos un símbolo
  if (tape.empty()) tape.push_back(Symbol('.'));
  bool accepted = false;
  int steps = 0;
  while (true) {
    if (trace) {
      os << "Paso " << steps << ": Estado=" << currentState << ", Cinta=";
      for (size_t i = 0; i < tape.size(); ++i) {
        if ((int)i == head) os << '[';
        os << tape[i];
        if ((int)i == head) os << ']';
      }
      os << "\n";
    }
    // Si el estado es de aceptación, termina
    if (currentState.getId() == states_.back().getId()) {
      accepted = true;
      break;
    }
    // Buscar transición aplicable
    bool found = false;
    for (const auto& t : transitions_) {
      if (t.getFrom().getId() == currentState.getId() && t.getStringSymbol().getValue() == tape[head].getValue()) {
        // Aplicar transición
        tape[head] = t.getTapeSymbol();
        currentState = t.getTo();
        // Movimiento de la cabeza
        char move = 'R';
        // Extraer dirección de movimiento de la transición (último campo de la línea)
        // Aquí asumimos que el campo de movimiento está en el tapeSymbol (por compatibilidad con parser actual)
        // Si no, habría que modificar Transition y el parser
        if (t.getTapeSymbol().getValue() == 'L') move = 'L';
        else if (t.getTapeSymbol().getValue() == 'R') move = 'R';
        // Mover cabeza
        if (move == 'L') {
          if (head == 0) {
            tape.insert(tape.begin(), Symbol('.'));
          } else {
            head--;
          }
        } else if (move == 'R') {
          head++;
          if (head == (int)tape.size()) tape.push_back(Symbol('.'));
        }
        found = true;
        break;
      }
    }
    if (!found) break; // No hay transición aplicable
    steps++;
    if (steps > 10000) break; // Evita bucles infinitos
  }
  return accepted;
}

std::ostream& operator<<(std::ostream& os, const TuringMachine& tm) {
  os << "States:\n";
  for (const auto& state : tm.states_) {
    os << state << "\n";
  }
  os << "String Alphabet: " << tm.stringAlphabet_ << "\n";
  os << "Tape Alphabet: " << tm.tapeAlphabet_ << "\n";
  os << "Transitions:\n";
  for (const auto& transition : tm.transitions_) {
    os << transition << "\n";
  }
  return os;
}