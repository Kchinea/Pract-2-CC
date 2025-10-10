#include <iostream>
#include "usage.h"
#include "arg_parser.h"
#include "file_parser.h"
#include "turing_machine.h"

int main (int argc, char* argv[]) {
  usage(argc, argv);
  auto [fileIn, fileOut, flag] = parse_args(argc, argv);
  std::string inputFile = argv[1];
  FileParser parser;
  TuringMachine machine = parser.parseFile(fileIn);
  std::cout << machine << std::endl;
  std::string inputString;
  std::cout << "Ingrese la cadena a evaluar: ";
  std::cin >> inputString;
  std::vector<Symbol> symbols;
  for (char c : inputString) {
    symbols.push_back(Symbol(c));
  }
  String str(symbols);
  if (machine.compute(str, flag, std::cout)) {
    std::cout << "La cadena es aceptada por el autómata de pila." << std::endl;
  } else {
    std::cout << "La cadena no es aceptada por el autómata de pila." << std::endl;
  } 
  return 0;
}