#include "turing_machine.h"
#include <iostream> 

bool TuringMachine::compute(String& input, bool trace, std::ostream& os) const {
  // Multi-tape Turing Machine simulation
  // Initialize tapes: start with one tape from input, but number of tapes is inferred
  // from transitions (max of writeSymbols size and readSymbols size). For simplicity,
  // we will determine tape count from the first transition if available.
  int tapeCount = determineTapeCount();
  std::vector<std::vector<Symbol>> tapes;
  std::vector<int> heads;
  initializeTapes(input, tapeCount, tapes, heads);

  State currentState;
  if (!states_.empty()) currentState = states_.front();

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
  // std::cout << "DEBUG: Estado final: " << currentState << " DEBUG: Estado aceptacion: " << currentState.isAccept() << std::endl<< std::endl<< std::endl<< std::endl;
  return accepted;
}

int TuringMachine::determineTapeCount() const {
  int tapeCount = 1;
  for (const auto & tr : transitions_) {
    tapeCount = std::max(tapeCount, (int)tr.getReadSymbols().size());
    tapeCount = std::max(tapeCount, (int)tr.getWriteSymbols().size());
    tapeCount = std::max(tapeCount, (int)tr.getMovements().size());
  }
  return tapeCount;
}

void TuringMachine::initializeTapes(const String& input, int tapeCount, std::vector<std::vector<Symbol>>& tapes, std::vector<int>& heads) const {
  tapes.assign(tapeCount, std::vector<Symbol>());
  heads.assign(tapeCount, 0);
  for (const auto& s : input.getSymbols()) tapes[0].push_back(s);
  if (tapes[0].empty()) tapes[0].push_back(Symbol('.'));
  for (int i = 1; i < tapeCount; ++i) tapes[i].push_back(Symbol('.'));
}

std::vector<Symbol> TuringMachine::readCurrentSymbols(const std::vector<std::vector<Symbol>>& tapes, const std::vector<int>& heads) const {
  int tapeCount = tapes.size();
  std::vector<Symbol> currentRead(tapeCount, Symbol('.'));
  for (int t = 0; t < tapeCount; ++t) {
    int h = heads[t];
    if (h >= 0 && h < (int)tapes[t].size()) currentRead[t] = tapes[t][h];
  }
  return currentRead;
}

const Transition* TuringMachine::findApplicableTransition(const State& currentState, const std::vector<std::vector<Symbol>>& tapes, const std::vector<int>& heads) const {
  int tapeCount = tapes.size();
  std::vector<Symbol> currentRead = readCurrentSymbols(tapes, heads);
  for (const auto& tr : transitions_) {
    if (tr.getFrom().getId() != currentState.getId()) continue;
    auto readSyms = tr.getReadSymbols();
    bool match = true;
    for (int t = 0; t < tapeCount; ++t) {
      char expected = (t < (int)readSyms.size() ? readSyms[t].getValue() : '.');
      if (currentRead[t].getValue() != expected) { match = false; break; }
    }
    if (match) return &tr;
  }
  return nullptr;
}

void TuringMachine::applyTransition(const Transition& tr, std::vector<std::vector<Symbol>>& tapes, std::vector<int>& heads, State& currentState) const {
  int tapeCount = tapes.size();
  auto writeSyms = tr.getWriteSymbols();
  for (int t = 0; t < tapeCount; ++t) {
    if (t < (int)writeSyms.size()) {
      tapes[t][heads[t]] = writeSyms[t];
    }
  }
  auto moves = tr.getMovements();
  for (int t = 0; t < tapeCount; ++t) {
    Moves mv = (t < (int)moves.size() ? moves[t] : Moves::STAY);
    if (mv == Moves::LEFT) {
      if (heads[t] == 0) tapes[t].insert(tapes[t].begin(), Symbol('.'));
      else heads[t]--;
    } else if (mv == Moves::RIGHT) {
      heads[t]++;
      if (heads[t] == (int)tapes[t].size()) tapes[t].push_back(Symbol('.'));
    }
  }
  std::string toId = tr.getTo().getId();
  for (const auto& s : states_) {
    if (s.getId() == toId) { currentState = s; break; }
  }
}

void TuringMachine::flattenResult(String& input, const std::vector<std::vector<Symbol>>& tapes) const {
  if (tapes.empty()) { input = String(std::vector<Symbol>()); return; }
  input = String(tapes[0]);
}

std::ostream& operator<<(std::ostream& os, const TuringMachine& tm) {
  os << "States:\n";
  for (const auto& state : tm.states_) {
    os << state;
    if (state.isAccept()) os << " (accept)";
    os << "\n";
  }
  os << "String Alphabet: " << tm.stringAlphabet_ << "\n";
  os << "Tape Alphabet: " << tm.tapeAlphabet_ << "\n";
  os << "Transitions:\n";
  for (const auto& transition : tm.transitions_) {
    os << transition << "\n";
  }
  return os;
}