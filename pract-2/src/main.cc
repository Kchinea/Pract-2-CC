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
  Args args = parse_args(argc, argv);
  FileParser parser;
  TuringMachine machine = parser.parseFile(args.mtFile);
  // Print the parsed Turing Machine to stdout
  // std::cout << machine << std::endl;
  std::ifstream inputStrings(args.stringsFile);
  if (!inputStrings.is_open()) {
    std::cerr << "No se pudo abrir el archivo de cadenas: " << args.stringsFile << std::endl;
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
    String str(symbols);
    std::ostringstream traceStream;
    bool accepted = machine.compute(str, args.trace, traceStream);
    resultFile << inputString << ": " << (accepted ? "ACEPTADA" : "RECHAZADA");
    // Also output how the string ended up on the tape after computation
    resultFile << " -> Resultado: ";
    resultFile << str << std::endl;
    if (args.trace) {
      resultFile << traceStream.str() << std::endl;
    }
  }
  inputStrings.close();
  resultFile.close();
  return 0;
}