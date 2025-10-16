#ifndef TURING_MACHINE_H
#define TURING_MACHINE_H

#include <vector>
#include "string.h"
#include "alphabet.h"
#include "state.h"
#include "transition.h"

class TuringMachine {
 public:
  TuringMachine() = default;
  TuringMachine(std::vector<State> states, std::vector<Transition> transitions, Alphabet stringAlphabet, Alphabet tapeAlphabet) :
    states_(states), transitions_(transitions), stringAlphabet_(stringAlphabet), tapeAlphabet_(tapeAlphabet) {}
  const std::vector<State>& getStates() const { return states_; }
  const std::vector<Transition>& getTransitions() const { return transitions_; }
  const Alphabet& getStringAlphabet() const { return stringAlphabet_; }
  const Alphabet& getTapeAlphabet() const { return tapeAlphabet_; }
  friend std::ostream& operator<<(std::ostream& os, const TuringMachine& tm);
  bool compute(String& input, bool trace, std::ostream& os) const;
 private:
  // helper methods to keep compute small
  int determineTapeCount() const;
  void initializeTapes(const String& input, int tapeCount, std::vector<std::vector<Symbol>>& tapes, std::vector<int>& heads) const;
  std::vector<Symbol> readCurrentSymbols(const std::vector<std::vector<Symbol>>& tapes, const std::vector<int>& heads) const;
  const Transition* findApplicableTransition(const State& currentState, const std::vector<std::vector<Symbol>>& tapes, const std::vector<int>& heads) const;
  void applyTransition(const Transition& tr, std::vector<std::vector<Symbol>>& tapes, std::vector<int>& heads, State& currentState) const;
  void flattenResult(String& input, const std::vector<std::vector<Symbol>>& tapes) const;
 private:
  std::vector<State> states_;
  std::vector<Transition> transitions_;
  Alphabet stringAlphabet_;
  Alphabet tapeAlphabet_;
};

#endif