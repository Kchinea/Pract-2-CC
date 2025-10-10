#ifndef STRING_H
#define STRING_H

#include "symbol.h"
#include <vector>

class String {
  public:
    String(const std::vector<Symbol>& symbols) : symbols(symbols) {}
    const std::vector<Symbol>& getSymbols() const { return symbols; }
    friend std::ostream& operator<<(std::ostream& os, const String& string);
    int size() const { return symbols.size(); }
    bool empty() const { return symbols.empty(); }
    Symbol operator[](size_t index) const { return symbols[index]; }
  private:
    std::vector<Symbol> symbols;
};

#endif
