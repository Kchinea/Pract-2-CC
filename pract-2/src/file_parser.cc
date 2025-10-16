#include "file_parser.h"


FileParser::FileParser() {}
FileParser::~FileParser() {}


std::vector<std::string> FileParser::parseTokens(const std::string& line) {
	std::vector<std::string> tokens;
	std::stringstream ss(line);
	std::string token;
	while (ss >> token) {
		tokens.push_back(token);
	}
	return tokens;
}

std::string FileParser::trim(const std::string& s) {
	std::string res = s;
	res.erase(0, res.find_first_not_of(" \t\n\r"));
	res.erase(res.find_last_not_of(" \t\n\r") + 1);
	return res;
}

bool FileParser::readAndSplitSections(std::ifstream& infile,
	std::vector<std::string>& states,
	std::vector<std::string>& inputAlphabet,
	std::vector<std::string>& tapeAlphabet,
	std::string& initialState,
	std::string& initialStackSymbol,
	std::vector<std::string>& acceptStates,
	std::vector<std::string>& transitions)
{
	std::string line;
	int section = 0;
	while (std::getline(infile, line)) {
		size_t comment = line.find('#');
		if (comment != std::string::npos) line = line.substr(0, comment);
		line = trim(line);
		if (line.empty()) continue;

		switch (section) {
			case 0: states = parseTokens(line); break;
			case 1: inputAlphabet = parseTokens(line); break;
			case 2: tapeAlphabet = parseTokens(line); break;
			case 3: initialState = line; break;
			case 4: initialStackSymbol = line; break;
			case 5: acceptStates = parseTokens(line); break;
			default: transitions.push_back(line); break;
		}
		section++;
	}
	return true;
}

Alphabet FileParser::buildAlphabet(const std::vector<std::string>& symbols) {
	Alphabet alpha;
	for (const auto& symbol : symbols) {
		if (!symbol.empty()) alpha.addSymbol(symbol[0]);
	}
	return alpha;
}

std::vector<State> FileParser::buildStates(const std::vector<std::string>& stateNames, const std::vector<std::string>& acceptStates) {
	std::vector<State> stateObjects;
	for (const auto& stateName : stateNames) {
		State state(stateName);
		stateObjects.push_back(state);
	}
	for (auto & st : stateObjects) {
		for (const auto & acc : acceptStates) {
			if (st.getId() == acc) st.setAccept(true);
		}
	}
	return stateObjects;
}

std::vector<Transition> FileParser::parseTransitionLines(const std::vector<std::string>& transitions) {
	std::vector<Transition> transitionObjects;
	for (const auto& transLine : transitions) {
		std::stringstream ss(transLine);
		std::string fromState, readSymStr, toState;
		ss >> fromState >> readSymStr >> toState;
		State from(fromState);
		State to(toState);

		std::vector<Symbol> readVec;
		if (!readSymStr.empty()) readVec.push_back(Symbol(readSymStr[0]));
		std::vector<Symbol> writeVec;
		std::vector<Moves> movesVec;
		std::string writeSymStr, moveStr;
		while (ss >> writeSymStr >> moveStr) {
			if (!writeSymStr.empty()) writeVec.push_back(Symbol(writeSymStr[0]));
			if (!moveStr.empty()) {
				char m = moveStr[0];
				if (m == 'L') movesVec.push_back(Moves::LEFT);
				else if (m == 'R') movesVec.push_back(Moves::RIGHT);
				else movesVec.push_back(Moves::STAY);
			}
		}
		Transition transition(from, to, readVec, writeVec, movesVec);
		transitionObjects.push_back(transition);
	}
	return transitionObjects;
}

TuringMachine FileParser::parseFile(const std::string& filename) {
	std::ifstream infile(filename);
	if (!infile.is_open()) {
		std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
		return TuringMachine();
	}

	std::vector<std::string> states, inputAlphabet, tapeAlphabet, transitions, acceptStates;
	std::string initialState, initialStackSymbol;

	readAndSplitSections(infile, states, inputAlphabet, tapeAlphabet, initialState, initialStackSymbol, acceptStates, transitions);
	infile.close();

	Alphabet inputAlpha = buildAlphabet(inputAlphabet);
	Alphabet tapeAlpha = buildAlphabet(tapeAlphabet);

	std::vector<State> stateObjects = buildStates(states, acceptStates);
	// initial state object can be used if needed
	State initialStateObj(initialState);

	std::vector<Transition> transitionObjects = parseTransitionLines(transitions);

	TuringMachine machine(stateObjects, transitionObjects, inputAlpha, tapeAlpha);
	return machine;
}
