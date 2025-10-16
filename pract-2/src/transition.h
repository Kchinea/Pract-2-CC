#ifndef TRANSITION_H
#define TRANSITION_H

#include "state.h"
#include "symbol.h"
#include "moves.h"
#include <vector>

class Transition {
  public:
    Transition(const State& from, const State& to, const std::vector<Symbol>& readSymbols, std::vector<Symbol>& writeSymbols, const std::vector<Moves>& movements)
      : from(from), to(to), readSymbols(readSymbols), writeSymbols(writeSymbols), movements(movements)
       {}

  State getFrom() const { return from; }
  State getTo() const { return to; }
  std::vector<Symbol> getReadSymbols() const { return readSymbols; }
  std::vector<Symbol> getWriteSymbols() const { return writeSymbols; }
  std::vector<Moves> getMovements() const { return movements; }
    friend std::ostream& operator<<(std::ostream& os, const Transition& t);
  private:
    State from;
    State to;
    std::vector<Symbol> readSymbols;
    std::vector<Symbol> writeSymbols;
    std::vector<Moves> movements;
};

#endif