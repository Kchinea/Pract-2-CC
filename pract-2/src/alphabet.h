#ifndef ALPHABET_H
#define ALPHABET_H

#include <set>
#include "symbol.h"
#include <ostream>

class Alphabet {
  public: 
  Alphabet() = default;
  Alphabet(const std::set<Symbol>& symbols) : symbols(symbols) {}
  void addSymbol(const Symbol& symbol) {
    symbols.insert(symbol);
  }
  
  const std::set<Symbol>& getSymbols() const {
    return symbols;
  }
  bool contains(const Symbol& symbol) const {
    return symbols.find(symbol) != symbols.end();
  }
  friend std::ostream& operator<<(std::ostream& os, const Alphabet& alphabet);
  private:
  std::set<Symbol> symbols;
};

#endif


