#include "transition.h"

std::ostream& operator<<(std::ostream& os, const Transition& t){
  os << "Transition(" 
    << t.getFrom() << " -> " << t.getTo() 
    << ", input: " << t.getStringSymbol() 
    << ", stack: " << t.getStackSymbol()
    << ", push: {";
  for (size_t i = 0; i < t.getStackPushSymbols().size(); ++i) {
    if (i > 0)
      os << ", ";
    os << t.getStackPushSymbols()[i];
  }
  os << "})";
  return os;
}