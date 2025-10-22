#ifndef TRACE_PRINTER_H
#define TRACE_PRINTER_H

#include <ostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>
#include "state.h"
#include "symbol.h"
#include "transition.h"
#include "moves.h"

/**
 * @brief Clase responsable de imprimir la traza de ejecución de la Máquina de Turing.
 * 
 * Se encarga exclusivamente del formateo y presentación de la información de traza,
 * separando esta responsabilidad del simulador. Sigue el principio de Single Responsibility.
 */
class TracePrinter {
 public:
  /**
   * @brief Imprime el encabezado de la traza de ejecución.
   * 
   * @param os Stream de salida donde se imprimirá el encabezado.
   */
  void printHeader(std::ostream& os) const;
  
  /**
   * @brief Imprime la información de un paso de la ejecución.
   * 
   * Muestra el número de paso, el estado actual, los símbolos leídos de cada cinta,
   * la transición aplicada (si existe), las acciones que se realizarán y el estado
   * actual de todas las cintas con la posición de los cabezales.
   * 
   * @param os Stream de salida donde se imprimirá el paso.
   * @param step Número del paso actual.
   * @param currentState Estado actual de la máquina.
   * @param currentRead Vector con los símbolos leídos de cada cinta.
   * @param tr Transición que se aplicará.
   * @param foundTransition Indica si se encontró una transición aplicable.
   * @param tapes Vector de cintas en su estado actual.
   * @param heads Vector con las posiciones de los cabezales.
   * @param tapeCount Número total de cintas.
   */
  void printStep(std::ostream& os, int step, 
                const State& currentState,
                const std::vector<Symbol>& currentRead, 
                const Transition& tr, bool foundTransition,
                const std::vector<std::vector<Symbol>>& tapes,
                const std::vector<int>& heads, int tapeCount) const;
  
  /**
   * @brief Imprime el mensaje cuando se alcanza un estado de aceptación.
   * 
   * @param os Stream de salida donde se imprimirá el mensaje.
   */
  void printAcceptedMessage(std::ostream& os) const;
  
  /**
   * @brief Imprime el mensaje cuando no se encuentra transición aplicable.
   * 
   * @param os Stream de salida donde se imprimirá el mensaje.
   */
  void printRejectedMessage(std::ostream& os) const;
  
  /**
   * @brief Imprime el mensaje cuando se excede el límite de pasos.
   * 
   * @param os Stream de salida donde se imprimirá el mensaje.
   */
  void printMaxStepsMessage(std::ostream& os) const;
};

#endif
