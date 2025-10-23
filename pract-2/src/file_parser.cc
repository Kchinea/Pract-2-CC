#include "file_parser.h"
#include <stdexcept>

/**
 * @brief Constructor por defecto de FileParser.
 */
FileParser::FileParser() {}

/**
 * @brief Destructor de FileParser.
 */
FileParser::~FileParser() {}

/**
 * @brief Parsea una línea y extrae todos los tokens separados por espacios.
 * 
 * @param line Línea a parsear.
 * @return Vector de strings con los tokens extraídos.
 */
std::vector<std::string> FileParser::parseTokens(const std::string& line) {
	std::vector<std::string> tokens;
	std::stringstream stream(line);
	std::string token;
	while (stream >> token) {
		tokens.push_back(token);
	}
	return tokens;
}

/**
 * @brief Elimina espacios en blanco al inicio y final de un string.
 * 
 * @param s String a limpiar.
 * @return String sin espacios al inicio ni al final.
 */
std::string FileParser::trim(const std::string& line) {
	std::string result = line;
	result.erase(0, result.find_first_not_of(" \t\n\r"));
	result.erase(result.find_last_not_of(" \t\n\r") + 1);
	return result;
}

/**
 * @brief Lee el archivo de la MT y divide su contenido en secciones.
 * 
 * Parsea el fichero línea por línea, ignorando comentarios (líneas con #)
 * y líneas vacías. Divide el contenido en las secciones correspondientes:
 * estados, alfabetos, estado inicial, símbolo blanco, estados de aceptación
 * y transiciones.
 * 
 * @param infile Stream de entrada del fichero ya abierto.
 * @param states Vector donde se guardarán los nombres de los estados.
 * @param inputAlphabet Vector donde se guardará el alfabeto de entrada.
 * @param tapeAlphabet Vector donde se guardará el alfabeto de cinta.
 * @param initialState String donde se guardará el estado inicial.
 * @param initialStackSymbol String donde se guardará el símbolo blanco.
 * @param acceptStates Vector donde se guardarán los estados de aceptación.
 * @param transitions Vector donde se guardarán las líneas de transiciones.
 * @return true si se leyó correctamente, false en caso contrario.
 */
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

/**
 * @brief Construye un objeto Alphabet a partir de un vector de strings.
 * 
 * Toma cada string del vector y extrae su primer carácter para añadirlo
 * al alfabeto.
 * 
 * @param symbols Vector de strings con los símbolos.
 * @return Objeto Alphabet construido.
 */
Alphabet FileParser::buildAlphabet(const std::vector<std::string>& symbols) {
	Alphabet alpha;
	for (const auto& symbol : symbols) {
		if (!symbol.empty()) alpha.addSymbol(symbol[0]);
	}
	return alpha;
}

/**
 * @brief Construye un vector de objetos State a partir de nombres y estados de aceptación.
 * 
 * Crea un objeto State por cada nombre en stateNames y marca como estados de
 * aceptación aquellos cuyo id coincida con los nombres en acceptStates.
 * 
 * @param stateNames Vector con los nombres de todos los estados.
 * @param acceptStates Vector con los nombres de los estados de aceptación.
 * @return Vector de objetos State con los flags de aceptación configurados.
 */
std::vector<State> FileParser::buildStates(const std::vector<std::string>& stateNames, const std::vector<std::string>& acceptStates) {
	std::vector<State> stateObjects;
	for (const auto& stateName : stateNames) {
		State state(stateName);
		stateObjects.push_back(state);
	}
	for (auto & state_iter : stateObjects) {
		for (const auto & accept : acceptStates) {
			if ( state_iter.getId() == accept) state_iter.setAccept(true);
		}
	}
	return stateObjects;
}

/**
 * @brief Parsea las líneas de transiciones y construye objetos Transition.
 * 
 * Formato: <estado_origen> <lectura0> <estado_destino> <escritura0> <mov0> <lectura1> <escritura1> <mov1> ...
 * Para cada línea de transición:
 * 1. Lee estado origen
 * 2. Lee símbolo de la cinta 0
 * 3. Lee estado destino
 * 4. Lee tripletas intercaladas: escritura0, mov0, lectura1, escritura1, mov1, lectura2, ...
 * 
 * @param transitions Vector de strings con las líneas de transiciones.
 * @return Vector de objetos Transition construidos.
 */
std::vector<Transition> FileParser::parseTransitionLines(const std::vector<std::string>& transitions) {
	std::vector<Transition> transitionObjects;
	for (const auto& transLine : transitions) {
		std::stringstream stream(transLine);
		std::string fromState, readSym0Str, toState;
		stream >> fromState >> readSym0Str >> toState;
		State from(fromState);
		State to(toState);
		std::vector<Symbol> readSymbols;
		readSymbols.push_back((!readSym0Str.empty()) ? Symbol(readSym0Str[0]) : Symbol('.'));
		std::map<int, std::pair<Symbol, Moves>> tapeActions;
		std::string writeSymStr, moveStr, readSymStr;
		int tapeIndex = 0;
		if (stream >> writeSymStr >> moveStr) {
			Symbol writeSymbol = (!writeSymStr.empty()) ? Symbol(writeSymStr[0]) : Symbol('.');
			Moves move = Moves::STAY;
			if (!moveStr.empty()) {
				char m = moveStr[0];
				if (m == 'L') move = Moves::LEFT;
				else if (m == 'R') move = Moves::RIGHT;
				else if (m == 'S') move = Moves::STAY;
				else { 
					throw std::range_error("Error: Movimiento inválido '" + moveStr + "' en la transición desde '" + fromState + "' a '" + toState + "'");
				}
			}
			tapeActions.insert(std::make_pair(tapeIndex, std::make_pair(writeSymbol, move)));
			tapeIndex++;
		}
		while (stream >> readSymStr >> writeSymStr >> moveStr) {
			Symbol readSym = (!readSymStr.empty()) ? Symbol(readSymStr[0]) : Symbol('.');
			readSymbols.push_back(readSym);
			Symbol writeSymbol = (!writeSymStr.empty()) ? Symbol(writeSymStr[0]) : Symbol('.');
			Moves move = Moves::STAY;
			if (!moveStr.empty()) {
				char m = moveStr[0];
				if (m == 'L') move = Moves::LEFT;
				else if (m == 'R') move = Moves::RIGHT;
				else if (m == 'S') move = Moves::STAY;
				else { 
					throw std::range_error("Error: Movimiento inválido '" + moveStr + "' en la transición desde '" + fromState + "' a '" + toState + "'");
				}
			
			}
			tapeActions.insert(std::make_pair(tapeIndex, std::make_pair(writeSymbol, move)));
			tapeIndex++;
		}
		Transition transition(from, to, readSymbols, tapeActions);
		transitionObjects.push_back(transition);
	}
	return transitionObjects;
}

/**
 * @brief Parsea un fichero de definición de máquina de Turing y construye el objeto TuringMachineModel.
 * 
 * Lee el fichero línea por línea, separa las secciones (estados, alfabetos,
 * estado inicial, símbolo blanco, estados de aceptación y transiciones),
 * construye los objetos necesarios (estados, alfabetos, transiciones) y
 * finalmente crea y retorna el objeto TuringMachineModel.
 * 
 * @param filename Ruta al fichero de definición de la MT.
 * @return Objeto TuringMachineModel construido a partir del fichero.
 */
TuringMachineModel FileParser::parseFile(const std::string& filename) {
	std::ifstream infile(filename);
	if (!infile.is_open()) {
		throw std::runtime_error("No se pudo abrir el archivo: " + filename);
	}
	std::vector<std::string> states, inputAlphabet, tapeAlphabet, transitions, acceptStates;
	std::string initialState, initialStackSymbol;
	readAndSplitSections(infile, states, inputAlphabet, tapeAlphabet, initialState, initialStackSymbol, acceptStates, transitions);
	infile.close();
	Alphabet inputAlpha = buildAlphabet(inputAlphabet);
	if(inputAlpha.contains(Symbol('.'))) {
		throw std::runtime_error("Error: El alfabeto de entrada contiene blanco.");
	}
	Alphabet tapeAlpha = buildAlphabet(tapeAlphabet);
	if(!tapeAlpha.contains(Symbol('.'))) {
		throw std::runtime_error("Error: El alfabeto de cinta no contiene blanco.");
	}
	std::vector<State> stateObjects = buildStates(states, acceptStates);
	State initialStateObj(initialState);
	std::vector<Transition> transitionObjects = parseTransitionLines(transitions);
	TuringMachineModel model(stateObjects, transitionObjects, inputAlpha, tapeAlpha, initialState);
	return model;
}
