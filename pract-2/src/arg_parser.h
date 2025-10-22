#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <string>

/**
 * @brief Clase para almacenar los argumentos parseados de línea de comandos.
 */
class Args {
public:
  std::string mtFile;
  std::string stringsFile;
  bool trace;
  Args(const std::string& mt, const std::string& strings, bool traceFlag)
    : mtFile(mt), stringsFile(strings), trace(traceFlag) {}
};

Args parse_args(int argc, char* argv[]);


#endif
