
#include "usage.h"

/**
 * @brief Valida los argumentos de línea de comandos y muestra ayuda si es necesario.
 * 
 * Verifica que se hayan pasado el número correcto de argumentos (2 o 3 además del nombre
 * del programa). Si se pasa --help como único argumento, muestra el mensaje de ayuda.
 * Si el número de argumentos es incorrecto, muestra un error y termina el programa.
 * 
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Array de argumentos de línea de comandos.
 */
void usage(int argc, char* argv[]) {
  const std::string helpMsg =
    "Uso: " + std::string(argv[0]) + " <arg1> <arg2> <arg3> (opcional) \n"
    "Este programa realiza una operación con 2 o 3 argumentos.\n";

  if (argc == 2 && std::string(argv[1]) == "--help") {
    std::cout << helpMsg;
    return;
  }
  if (argc < 3) {
    std::cerr << "Faltan argumentos. Se requieren 2 o 3 argumentos.\n";
    std::cerr << helpMsg;
    exit(EXIT_FAILURE);
  }
  if (argc > 4) {
    std::cerr << "Sobran argumentos. Solo se requieren 4 argumentos.\n";
    std::cerr << helpMsg;
    exit(EXIT_FAILURE);
  }
}