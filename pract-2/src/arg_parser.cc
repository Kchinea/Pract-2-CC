#include "arg_parser.h"


std::tuple<std::string, std::string, bool> parse_args(int argc, char* argv[]) {
  std::string fileIn = argv[1];
  std::string fileOut = argv[2];
  bool flag = (argc == 4 && std::string(argv[3]) == "--trace");
  return std::make_tuple(fileIn, fileOut, flag);
}
