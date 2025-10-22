#include "arg_parser.h"

/**
 * @brief Parsea los argumentos de línea de comandos.
 * 
 * Extrae el nombre del fichero de la MT, el fichero de cadenas y el flag de traza.
 * El formato esperado es: programa <fichero_MT> <fichero_cadenas> [--trace]
 * 
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Array de argumentos de línea de comandos.
 * @return Objeto Args con los argumentos parseados (mtFile, stringsFile, trace).
 */
Args parse_args(int argc, char* argv[]) {
  std::string mtFile = argv[1];
  std::string stringsFile = argv[2];
  bool traceFlag = (argc == 4 && std::string(argv[3]) == "--trace");
  return Args(mtFile, stringsFile, traceFlag);
}
