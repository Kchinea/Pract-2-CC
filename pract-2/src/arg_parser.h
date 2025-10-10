#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <string>
#include <tuple>

std::tuple<std::string, std::string, bool> parse_args(int argc, char* argv[]);


#endif
