#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <string>
#include <vector>
#include "alphabet.h"
#include "state.h"
#include "string.h"
#include "symbol.h"
#include "transition.h"
#include "turing_machine.h"
#include <fstream>
#include <iostream>
#include <sstream>



class FileParser {
public:
	FileParser();
	~FileParser();
	TuringMachine parseFile(const std::string& filename);
};

#endif
