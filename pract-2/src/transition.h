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
    // Constructor con vectores (compatibilidad con parser actual)
    Transition(const State& from, const State& to, const std::vector<Symbol>& readSymbols, 
               const std::vector<Symbol>& writeSymbols, const std::vector<Moves>& movements);

    // Constructor moderno con map unificado
    Transition(const State& from, const State& to, const Symbol& readSymbol,
               const std::map<int, std::pair<Symbol, Moves>>& tapeActions);

    // Getters
    const State& getFrom() const { return from; }
    const State& getTo() const { return to; }
    const Symbol& getReadSymbol() const { return readSymbol; }
    const std::map<int, std::pair<Symbol, Moves>>& getTapeActions() const { return tapeActions; }
    
    friend std::ostream& operator<<(std::ostream& os, const Transition& t);
    
  private:
    State from;
    State to;
    
    // Estructura optimizada
    Symbol readSymbol;  // Solo se lee de cinta 0
    std::map<int, std::pair<Symbol, Moves>> tapeActions;  // cinta -> (símbolo_a_escribir, movimiento)
};

#endif