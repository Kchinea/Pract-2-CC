#include "string.h"

std::ostream& operator<<(std::ostream& os, const String& string) {
  for (const auto& symbol : string.symbols) os << symbol;
  return os;
}