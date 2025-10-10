#ifndef SYMBOL_H
#define SYMBOL_H

#include <ostream>

class Symbol {
  public:
  Symbol(char value) : value(value) {}
  Symbol(const Symbol& sym) : value(sym.getValue()) {}
  char getValue() const { return value; }
  bool operator<(const Symbol& other) const { return value < other.value; }
  friend std::ostream& operator<<(std::ostream& os, const Symbol& symbol);
  private:
    char value;
};

#endif
