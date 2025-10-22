#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <string>

/**
 * @brief Clase para almacenar los argumentos parseados de línea de comandos.
 */
class Args {
public:
  std::string mtFile;      ///< Fichero de definición de la Máquina de Turing
  std::string stringsFile; ///< Fichero de cadenas a procesar
  bool trace;              ///< Flag para activar el modo traza
  
  /**
   * @brief Constructor de Args.
   * @param mt Fichero de definición de la MT.
   * @param strings Fichero de cadenas.
   * @param traceFlag Flag de traza.
   */
  Args(const std::string& mt, const std::string& strings, bool traceFlag)
    : mtFile(mt), stringsFile(strings), trace(traceFlag) {}
};

Args parse_args(int argc, char* argv[]);


#endif
