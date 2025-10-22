# Arquitectura del Simulador de Máquina de Turing

## Diagrama de Clases

```
┌─────────────────────────────────────────────────────────────┐
│                      TuringMachine                          │
│                   (Facade/Wrapper)                          │
├─────────────────────────────────────────────────────────────┤
│ - model_: unique_ptr<TuringMachineModel>                    │
│ - simulator_: unique_ptr<TuringMachineSimulator>            │
├─────────────────────────────────────────────────────────────┤
│ + TuringMachine(states, transitions, ...)                   │
│ + compute(input, trace, os): bool                           │
│ + getStates(): map&                                         │
│ + getTransitions(): map&                                    │
└──────────────┬──────────────────────────┬───────────────────┘
               │                          │
               │ owns                     │ owns
               │                          │
               ▼                          ▼
┌──────────────────────────┐   ┌──────────────────────────────┐
│  TuringMachineModel      │   │  TuringMachineSimulator      │
│  (Data/Structure)        │◄──┤  (Behavior/Logic)            │
├──────────────────────────┤   ├──────────────────────────────┤
│ - statesMap_             │   │ - model_: const Model&       │
│ - transitionsMap_        │   │ - tracePrinter_: TracePrinter│
│ - stringAlphabet_        │   ├──────────────────────────────┤
│ - tapeAlphabet_          │   │ + compute(input, ...): bool  │
│ - initialStateId_        │   │ - initializeTapes()          │
├──────────────────────────┤   │ - readCurrentSymbols()       │
│ + getStateById()         │   │ - findApplicableTransition() │
│ + getTransitionsFrom()   │   │ - applyTransition()          │
│ + getInitialState()      │   │ - flattenResult()            │
│ + determineTapeCount()   │   └──────────┬───────────────────┘
└──────────────────────────┘              │
                                          │ has-a
                                          │ (composición)
                                          ▼
                               ┌──────────────────────────┐
                               │     TracePrinter         │
                               │   (Presentation)         │
                               ├──────────────────────────┤
                               │ + printHeader()          │
                               │ + printStep()            │
                               │ + printAcceptedMessage() │
                               │ + printRejectedMessage() │
                               │ + printMaxStepsMessage() │
                               └──────────────────────────┘
```

## Principio de Diseño: Single Responsibility

### Evolución de la Arquitectura

#### **Versión 1: Monolítico**
- **TuringMachine**: Mezclaba definición de la MT + lógica de simulación
- Difícil de testear y extender
- Alta complejidad

#### **Versión 2: Modelo-Simulador**
- **TuringMachineModel**: Representa la estructura formal de la MT
- **TuringMachineSimulator**: Ejecuta la lógica de simulación
- Mejor pero aún con código de presentación mezclado

#### **Versión 3: Actual (Modelo-Simulador-Presentador)**

**TuringMachineModel** (Modelo de Datos)
- **Responsabilidad única**: Representar la estructura formal de la MT
- Almacena: estados, transiciones, alfabetos, estado inicial
- Proporciona acceso eficiente (O(1)) a través de maps
- **No sabe** cómo se ejecuta una simulación ni cómo se presenta

#### **TuringMachineSimulator** (Lógica de Simulación)
- **Responsabilidad única**: Ejecutar la simulación
- Mantiene el estado runtime: cintas, cabezas, estado actual
- Aplica transiciones y determina aceptación/rechazo
- **No posee** los datos de la MT, solo referencia al modelo

#### **TuringMachine** (Facade)
- **Responsabilidad única**: Proporcionar interfaz simple al exterior
- Mantiene compatibilidad hacia atrás
- Delega operaciones a Model o Simulator según corresponda
- Gestiona el ciclo de vida (ownership) de Model y Simulator

## Ventajas de esta Arquitectura

### 1. **Testabilidad**
```cpp
// Puedes testear el modelo independientemente
TuringMachineModel model(states, transitions, ...);
assert(model.determineTapeCount() == 2);

// Puedes testear el simulador con diferentes modelos
TuringMachineSimulator sim(model);
String input("010");
assert(sim.compute(input, false, std::cout) == true);
```

### 2. **Extensibilidad**
- Fácil añadir otros tipos de simuladores:
  - `TuringMachineDebugSimulator`: con breakpoints
  - `TuringMachineOptimizedSimulator`: con caché de transiciones
  - `TuringMachineParallelSimulator`: para MT no deterministas
- Fácil añadir validación/serialización al modelo sin afectar simulación

### 3. **Reutilización**
```cpp
TuringMachineModel model = parser.parse("machine.txt");

// Simular múltiples cadenas sin reconstruir el modelo
TuringMachineSimulator sim(model);
sim.compute(input1, false, os);
sim.compute(input2, false, os);
```

### 4. **Claridad**
- Separación clara entre "qué es la MT" (Model) y "cómo se ejecuta" (Simulator)
- Más fácil de entender y mantener
- Menos acoplamiento entre componentes

## Optimizaciones Implementadas

### Acceso a Estados: O(n) → O(1)
```cpp
// ANTES: búsqueda lineal
for (const auto& s : states_) {
  if (s.getId() == toId) { ... }  // O(n)
}

// AHORA: mapa hash
auto it = statesMap_.find(toId);   // O(1)
```

### Búsqueda de Transiciones: O(n) → O(1)
```cpp
// ANTES: iterar todas las transiciones
for (const auto& tr : transitions_) {  // O(n total)
  if (tr.getFrom().getId() != currentState) continue;
  // ...
}

// AHORA: solo las transiciones del estado actual
auto& transitions = transitionsMap_[currentState];  // O(1)
for (const auto& tr : transitions) {  // O(k) donde k << n
  // ...
}
```

### Getters sin Copias
```cpp
// ANTES: copia completa del vector
std::vector<Symbol> getReadSymbols() const { return readSymbols; }

// AHORA: referencia constante
const std::vector<Symbol>& getReadSymbols() const { return readSymbols; }
```

## Complejidad Final

- **Construcción del modelo**: O(n + m) donde n=estados, m=transiciones
- **Búsqueda de transición por paso**: O(1) acceso + O(k) matching, k=transiciones desde estado actual
- **Actualización de estado**: O(1) lookup en hash map
- **Simulación completa**: O(pasos × k × cintas)

## Ejemplo de Uso Avanzado

```cpp
// Parser crea vectores desde archivo
auto states = parser.parseStates();
auto transitions = parser.parseTransitions();

// Crear modelo (inmutable después de construcción)
TuringMachineModel model(states, transitions, inputAlpha, tapeAlpha);

// Validar el modelo
if (model.getStates().empty()) {
  std::cerr << "Error: sin estados" << std::endl;
}

// Crear simuladores especializados
TuringMachineSimulator standardSim(model);
TuringMachineDebugSimulator debugSim(model);  // Futuro

// Ejecutar
String input("0101");
bool accepted = standardSim.compute(input, true, std::cout);

// Reutilizar el mismo modelo con diferentes entradas
String input2("1100");
accepted = standardSim.compute(input2, false, nullStream);
```
