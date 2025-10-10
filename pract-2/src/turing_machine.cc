#include "turing_machine.h"

bool TuringMachine::compute(String& input, bool trace, std::ostream& os) const {
  // Implementation of the Turing machine computation goes here.
  // This is a placeholder implementation.
  if (trace) {
    os << "Tracing is enabled.\n";
  }
  os << "Input: " << input << "\n";
  // For now, we just return false to indicate rejection.
  return false;
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