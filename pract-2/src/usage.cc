
#include "usage.h"

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