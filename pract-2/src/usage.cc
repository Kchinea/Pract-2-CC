
#include "usage.h"

/**
 * @brief Valida los argumentos de línea de comandos y muestra ayuda si es necesario.
 * 
 * Verifica que se hayan pasado el número correcto de argumentos (2 o más además del nombre
 * del programa). Si se pasa --help como único argumento, muestra el mensaje de ayuda.
 * Si el número de argumentos es incorrecto, muestra un error y termina el programa.
 * 
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Array de argumentos de línea de comandos.
 */
void usage(int argc, char* argv[]) {
  const std::string helpMsg =
    "Uso: " + std::string(argv[0]) + " <fichero_MT> <fichero_cadenas> [--trace] [--info]\n"
    "  <fichero_MT>       : Archivo con la definición de la Máquina de Turing\n"
    "  <fichero_cadenas>  : Archivo con las cadenas de entrada a procesar\n"
    "  --trace (opcional) : Muestra la traza de ejecución paso a paso\n"
    "  --info  (opcional) : Muestra la información de la Máquina de Turing creada\n";

  if (argc == 2 && std::string(argv[1]) == "--help") {
    std::cout << helpMsg;
    exit(EXIT_SUCCESS);
  }
  if (argc < 3) {
    std::cerr << "Faltan argumentos. Se requieren al menos 2 argumentos.\n";
    std::cerr << helpMsg;
    exit(EXIT_FAILURE);
  }
  if (argc > 5) {
    std::cerr << "Demasiados argumentos. Máximo 4 argumentos (fichero_MT, fichero_cadenas, --trace, --info).\n";
    std::cerr << helpMsg;
    exit(EXIT_FAILURE);
  }
}