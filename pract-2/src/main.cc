#include <iostream>
#include "usage.h"
#include "arg_parser.h"
#include "file_parser.h"
#include "turing_machine.h"

/**
 * @brief Punto de entrada principal del simulador de Máquinas de Turing multicinta.
 * 
 * Flujo de ejecución:
 * 1. Valida los argumentos de línea de comandos
 * 2. Parsea los argumentos para obtener ficheros de entrada/salida y flag de traza
 * 3. Lee y construye la definición de la Máquina de Turing desde el fichero de entrada
 * 4. Lee cada cadena del fichero de cadenas
 * 5. Simula cada cadena en la MT, determinando si es aceptada o rechazada
 * 6. Escribe los resultados en FileOut.txt, incluyendo el estado final de la cinta 0
 * 7. Si el flag de traza está activo, incluye la traza completa de ejecución
 * 
 * @param argc Número de argumentos de línea de comandos
 * @param argv Array de argumentos de línea de comandos
 * @return 0 si la ejecución fue exitosa, 1 si hubo error abriendo ficheros
 */
int main (int argc, char* argv[]) {
  usage(argc, argv);
  Args args(argc, argv);
  FileParser parser;
  TuringMachine machine = parser.parseFile(args.getMtFile());
  if (args.getInfo()) {
    std::cout << "\n" << std::string(120, '=') << "\n";
    std::cout << "INFORMACIÓN DE LA MÁQUINA DE TURING\n";
    std::cout << std::string(120, '=') << "\n";
    std::cout << machine << "\n";
    std::cout << std::string(120, '=') << "\n\n";
  }
  std::ifstream inputStrings(args.getStringsFile());
  if (!inputStrings.is_open()) {
    std::cerr << "No se pudo abrir el archivo de cadenas: " << args.getStringsFile() << std::endl;
    return 1;
  }
  std::ofstream resultFile("FileOut.txt");
  std::string inputString;

  while (std::getline(inputStrings, inputString)) {
    if (inputString.empty()) continue;
    std::vector<Symbol> symbols;
    for (char c : inputString) {
      if (c != ' ' && c != '\n' && c != '\r') symbols.push_back(Symbol(c));
    }
    String string(symbols);
    std::ostringstream traceStream;
    if (args.getTrace()) {
      std::cout << "\n" << std::string(120, '=') << "\n";
      std::cout << "PROCESANDO CADENA: \"" << inputString << "\"\n";
      std::cout << std::string(120, '=') << "\n";
    }
    bool accepted = machine.compute( string, args.getTrace(), traceStream);
    resultFile << inputString << ": " << (accepted ? "ACEPTADA" : "RECHAZADA");
    resultFile << " -> Resultado: ";
    resultFile << string << std::endl;
    if (args.getTrace()) {
      std::cout << traceStream.str();
      std::cout << "\n" << std::string(120, '=') << "\n";
      std::cout << "RESULTADO FINAL: " << inputString << " -> " << (accepted ? "✓ ACEPTADA" : "✗ RECHAZADA") << "\n";
      std::cout << "Cinta resultante: " << string << "\n";
      std::cout << std::string(120, '=') << "\n\n";
    }
  }
  inputStrings.close();
  resultFile.close();
  if (!args.getTrace()) {
    std::cout << "Resultados guardados en FileOut.txt\n";
  }
  return 0;
}