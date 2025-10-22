#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <string>

/**
 * @brief Clase para almacenar los argumentos parseados de línea de comandos.
 */
class Args {
public:
  Args(int argc, char* argv[]);
  const std::string& getMtFile() const { return mtFile_; }
  const std::string& getStringsFile() const { return stringsFile_; }
  bool getTrace() const { return trace_; }
private:
  std::string mtFile_;
  std::string stringsFile_;
  bool trace_;
};



#endif
