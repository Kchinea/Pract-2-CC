#ifndef MOVES_H
#define MOVES_H

#include <ostream>

enum class Moves { LEFT, RIGHT, STAY };

inline std::ostream& operator<<(std::ostream& os, Moves m) {
  switch (m) {
    case Moves::LEFT:  
      os << 'L'; 
      break;
    case Moves::RIGHT: 
      os << 'R'; 
      break;
    case Moves::STAY: 
      os << 'S'; 
      break;
  }
  return os;
}

#endif
