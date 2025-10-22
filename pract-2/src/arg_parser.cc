#include "arg_parser.h"

/**
 * @brief Constructor que parsea los argumentos de línea de comandos.
 * 
 * Extrae el nombre del fichero de la MT, el fichero de cadenas y los flags de traza e info.
 * El formato esperado es: programa <fichero_MT> <fichero_cadenas> [--trace] [--info]
 * 
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Array de argumentos de línea de comandos.
 */
Args::Args(int argc, char* argv[]) 
  : mtFile_(argv[1]), 
    stringsFile_(argv[2]), 
    trace_(false),
    info_(false) {
  for (int i = 3; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--trace") trace_ = true;
    else if (arg == "--info") info_ = true;
  }
}
