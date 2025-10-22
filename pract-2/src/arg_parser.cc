#include "arg_parser.h"

/**
 * @brief Constructor que parsea los argumentos de línea de comandos.
 * 
 * Extrae el nombre del fichero de la MT, el fichero de cadenas y el flag de traza.
 * El formato esperado es: programa <fichero_MT> <fichero_cadenas> [--trace]
 * 
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Array de argumentos de línea de comandos.
 */
Args::Args(int argc, char* argv[]) 
  : mtFile_(argv[1]), 
    stringsFile_(argv[2]), 
    trace_(argc == 4 && std::string(argv[3]) == "--trace") {
}
