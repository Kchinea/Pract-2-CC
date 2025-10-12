#include "file_parser.h"


FileParser::FileParser() {}
FileParser::~FileParser() {}


static std::vector<std::string> parseTokens(const std::string& line) {
	std::vector<std::string> tokens;
	std::stringstream ss(line);
	std::string token;
	while (ss >> token) {
		tokens.push_back(token);
	}
	return tokens;
}

TuringMachine FileParser::parseFile(const std::string& filename) {
	std::ifstream infile(filename);
	if (!infile.is_open()) {
		std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
		return TuringMachine();
	}
	std::string line;
	int section = 0;
	std::vector<std::string> states, inputAlphabet, tapeAlphabet, transitions;
	std::string initialState, initialStackSymbol;

	while (std::getline(infile, line)) {
		size_t comment = line.find('#');
		if (comment != std::string::npos) line = line.substr(0, comment);
		// Elimina espacios al inicio y fin
		line.erase(0, line.find_first_not_of(" \t\n\r"));
		line.erase(line.find_last_not_of(" \t\n\r") + 1);
		if (line.empty()) continue;

		switch (section) {
			case 0:
				states = parseTokens(line);
				break;
			case 1:
				inputAlphabet = parseTokens(line);
				break;
			case 2:
				tapeAlphabet = parseTokens(line);
				break;
			case 3:
				initialState = line;
				break;
			case 4:
				initialStackSymbol = line;
				break;
			default:
				transitions.push_back(line);
				break;
		}
		section++;
	}
	infile.close();
	Alphabet inputAlpha;
	for (const auto& symbol : inputAlphabet) {
		inputAlpha.addSymbol(symbol[0]);
	}
	Alphabet tapeAlpha;
	for (const auto& symbol : tapeAlphabet) {
		tapeAlpha.addSymbol(symbol[0]);
	}

	std::vector<State> stateObjects;
	for (const auto& stateName : states) {
		State state(stateName);
		stateObjects.push_back(state);
	}
	State initialStateObj(initialState);

	Symbol initialStack(initialStackSymbol[0]);
	std::vector<Transition> transitionObjects;
	for (const auto& transLine : transitions) {
		std::stringstream ss(transLine);
		std::string fromState, stringSymbol, stackSymbol, toState, stackPushStr;
		ss >> fromState >> stringSymbol >> stackSymbol >> toState >> stackPushStr;
		State from(fromState);
		State to(toState);
		Symbol strSym(stringSymbol[0]);
		Symbol stkSym(stackSymbol[0]);
		std::vector<Symbol> stackPushSet;
		for (char c : stackPushStr) {
			if (c != '.') // Si el símbolo es epsilon, puedes manejarlo aparte
				stackPushSet.push_back(Symbol(c));
		}
		Transition transition(from, to, strSym, stkSym);
		transitionObjects.push_back(transition);
	}
	// Aquí deberías parsear las transiciones si tienes la clase Transition

	TuringMachine machine(stateObjects, transitionObjects, inputAlpha, tapeAlpha);
	return machine;
}
