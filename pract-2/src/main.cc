#include <iostream>
#include "usage.h"
#include "arg_parser.h"
#include "file_parser.h"
#include "turing_machine.h"

int main (int argc, char* argv[]) {
  usage(argc, argv);
  auto args = parse_args(argc, argv);
  auto fileIn = std::get<0>(args);
  auto fileOut = std::get<1>(args);
  auto flag = std::get<2>(args);
  FileParser parser;
  TuringMachine machine = parser.parseFile(fileIn);
  std::ifstream inputStrings(fileOut); // fileOut es el fichero de cadenas
  if (!inputStrings.is_open()) {
    std::cerr << "No se pudo abrir el archivo de cadenas: " << fileOut << std::endl;
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
    bool accepted = machine.compute(str, flag, traceStream);
    resultFile << inputString << ": " << (accepted ? "ACEPTADA" : "RECHAZADA") << std::endl;
    if (flag) {
      resultFile << traceStream.str() << std::endl;
    }
  }
  inputStrings.close();
  resultFile.close();
  return 0;
}