#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <string>
#include <vector>
#include "alphabet.h"
#include "state.h"
#include "string.h"
#include "symbol.h"
#include "transition.h"
#include "turing_machine_model.h"
#include <fstream>
#include <iostream>
#include <sstream>



class FileParser {
public:
	FileParser();
	~FileParser();
	std::vector<std::string> parseTokens(const std::string& line);
	std::string trim(const std::string& s);
	bool readAndSplitSections(std::ifstream& infile,
		std::vector<std::string>& states,
		std::vector<std::string>& inputAlphabet,
		std::vector<std::string>& tapeAlphabet,
		std::string& initialState,
		std::string& initialStackSymbol,
		std::vector<std::string>& acceptStates,
		std::vector<std::string>& transitions);
	Alphabet buildAlphabet(const std::vector<std::string>& symbols);	
	std::vector<State> buildStates(const std::vector<std::string>& stateNames, const std::vector<std::string>& acceptStates);
	std::vector<Transition> parseTransitionLines(const std::vector<std::string>& transitions);
	TuringMachineModel parseFile(const std::string& filename);

};

#endif
