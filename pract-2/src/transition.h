#ifndef TRANSITION_H
#define TRANSITION_H

#include "state.h"
#include "symbol.h"
#include "moves.h"
#include <map>
#include <vector>

/**
 * @brief Representa una transición de una Máquina de Turing multicinta.
 * 
 * Estructura optimizada usando map para acciones por cinta:
 * - Lee un símbolo de la cinta 0 (readSymbol)
 * - Para cada cinta: escribe símbolo y mueve cabeza (tapeActions)
 */
class Transition {
  public:
    Transition(const State& from, const State& to, const Symbol& readSymbol,
               const std::map<int, std::pair<Symbol, Moves>>& tapeActions);
    const State& getFrom() const { return from; }
    const State& getTo() const { return to; }
    const Symbol& getReadSymbol() const { return readSymbol; }
    const std::map<int, std::pair<Symbol, Moves>>& getTapeActions() const { return tapeActions; }
    
    friend std::ostream& operator<<(std::ostream& os, const Transition& t);
    
  private:
    State from;
    State to;
    Symbol readSymbol;
    std::map<int, std::pair<Symbol, Moves>> tapeActions;
};

#endif