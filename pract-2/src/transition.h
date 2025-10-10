#ifndef TRANSITION_H
#define TRANSITION_H

#include "state.h"
#include "symbol.h"
#include <vector>

class Transition {
  public:
    Transition(const State& from, const State& to, const Symbol& stringSymbol, const Symbol& tapeSymbol)
      : from(from), to(to), stringSymbol(stringSymbol), tapeSymbol(tapeSymbol)
       {}

    State getFrom() const { return from; }
    State getTo() const { return to; }
    Symbol getStringSymbol() const { return stringSymbol; }
    Symbol getTapeSymbol() const { return tapeSymbol; }
    friend std::ostream& operator<<(std::ostream& os, const Transition& t);
  private:
    State from;
    State to;
    Symbol stringSymbol;
    Symbol tapeSymbol;
};

#endif