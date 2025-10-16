#include "transition.h"

static void printSymbols(std::ostream& os, const std::vector<Symbol>& syms) {
  os << '[';
  for (size_t i = 0; i < syms.size(); ++i) {
    if (i) os << ' ';
    os << syms[i];
  }
  os << ']';
}

static void printMoves(std::ostream& os, const std::vector<Moves>& moves) {
  os << '[';
  for (size_t i = 0; i < moves.size(); ++i) {
    if (i) os << ' ';
    os << moves[i];
  }
  os << ']';
}

std::ostream& operator<<(std::ostream& os, const Transition& t){
  os << "Transition(" << t.getFrom() << " -> " << t.getTo() << ", read: ";
  printSymbols(os, t.getReadSymbols());
  os << ", write: ";
  printSymbols(os, t.getWriteSymbols());
  os << ", moves: ";
  printMoves(os, t.getMovements());
  os << ")";
  return os;
}