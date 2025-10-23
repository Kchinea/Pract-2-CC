# Práctica 2 — Simulador de Máquina de Turing Multicinta

Simulador de Máquinas de Turing multicinta en C++ con soporte completo para lectura y escritura simultánea en múltiples cintas.

## Índice

1. [Compilar y Ejecutar](#compilar-y-ejecutar)
2. [Estructura del Proyecto](#estructura-del-proyecto)
3. [Formato de Entrada](#formato-de-entrada)
4. [Arquitectura y Diseño](#arquitectura-y-diseño)
   - [Decisiones de Diseño Clave](#decisiones-de-diseño-clave)
   - [Clases Principales](#clases-principales)
   - [Flujo de Ejecución](#flujo-de-ejecución)
5. [Modos de Ejecución](#modos-de-ejecución)
6. [Resolución de Problemas](#resolución-de-problemas)
   - [Ejercicio 1: a^n b^m donde m > n](#ejercicio-1-an-bm-donde-m--n)
   - [Ejercicio 2: Contador de a's y b's](#ejercicio-2-contador-de-as-y-bs)
7. [Testing](#testing)
8. [Limitaciones y Características](#limitaciones-y-características)
9. [Ejemplos de Uso](#ejemplos-de-uso)

## Compilar y Ejecutar

```bash
make                    # Compila el proyecto
./pract-02 <MT.txt> <strings.txt> [--trace] [--info]
./test_all.sh          # Ejecuta todos los tests
```

**Flags opcionales:**
- `--trace`: muestra ejecución paso a paso con estado de todas las cintas
- `--info`: muestra estructura completa de la MT antes de ejecutar

## Estructura del Proyecto

```
pract-2/
├── src/               # Código fuente (24 archivos .h/.cc)
│   ├── main.cc        # Punto de entrada
│   ├── file_parser.*  # Parseo de archivos MT
│   ├── turing_machine_model.*     # Modelo de datos
│   ├── turing_machine_simulator.* # Motor de ejecución
│   ├── transition.*   # Transiciones multicinta
│   ├── state.*        # Estados de la MT
│   └── ...
├── Inputs/            # 2 ejemplos documentados (AnBn_Plus, CountAB)
├── Test/              # Suite de 9 MTs de prueba + resultados
├── Makefile           # Compilación automática
└── test_all.sh        # Script de testing automatizado
```

## Formato de Entrada

### Archivo de definición de MT (7 líneas + transiciones):

1. **Estados**: `q0 q1 q2 qaccept`
2. **Alfabeto de entrada**: `0 1 a b`
3. **Alfabeto de cinta**: `0 1 a b . X Y` (incluir blanco `.`)
4. **Estado inicial**: `q0`
5. **Símbolo blanco**: `.`
6. **Estados de aceptación**: `qaccept`
7. **Transiciones** (formato multicinta intercalado):

```
<estado_origen> <read0> <estado_destino> <write0> <mov0> <read1> <write1> <mov1> ...
```

**Ejemplo (2 cintas):**
```
q0 a q1 X R . . R
```
- Estado `q0`, lee `a` de cinta0 y `.` de cinta1
- Va a `q1`
- Escribe `X` en cinta0 y mueve derecha (R)
- Escribe `.` en cinta1 y mueve derecha (R)

**Movimientos**: `L` (left/izquierda), `R` (rigth/derecha), `S` (stay/quieto)

### Archivo de cadenas:
Una cadena por línea, sin espacios. Se carga en la cinta 0 al inicio.

## Arquitectura y Diseño

### Decisiones de Diseño Clave

**Problema inicial**: La implementación principal, la primera vez que implemente de la carteristica adicional solo leía de la cinta 0, ignorando las demás cintas en transiciones multicinta.

**Solución**: Realice una refactorizacion completa para lectura de **todas las cintas**:
1. `Transition` ahora almacena `vector<Symbol> readSymbols` (uno por cinta)
2. El parser lee el formato intercalado: `read0 write0 mov0 read1 write1 mov1...`
3. El simulador compara **todos** los símbolos de todas las cintas antes de aplicar una transición

**Arquitectura**: Separación clara entre Modelo (datos) y Simulador (ejecución)

### Clases Principales

#### **TuringMachineModel** (`turing_machine_model.h/cc`)
**Propósito**: Representa la definición formal de la MT (estados, transiciones, alfabetos).

**Responsabilidades**:
- Almacena estados en `map<string, State>` para búsqueda mas eficiente
- Indexa transiciones por estado origen: `map<string, vector<Transition>>`
- Gestiona alfabetos de entrada y cinta
- Provee métodos de consulta: `getState()`, `getTransitionsFrom()`, `isAcceptState()`

**Por qué existe**: Separar la estructura de la MT de su ejecución permite cargar múltiples MTs sin reiniciar el simulador.

#### **TuringMachineSimulator** (`turing_machine_simulator.h/cc`)
**Propósito**: Motor de ejecución que simula paso a paso la MT.

**Responsabilidades**:
- Mantiene estado de las cintas: `vector<vector<Symbol>> tapes` y `vector<int> heads`
- Ejecuta el bucle principal de simulación
- Busca transiciones aplicables comparando **todas** las cintas
- Aplica transiciones (escritura + movimiento de cabezales)
- Detecta bucles infinitos (MAX_STEPS = 50)

**Evolución**: Inicialmente solo comparaba cinta 0. Ahora itera sobre todas las `readSymbols` y las compara con `currentSymbols` de todas las cintas.

**Funciones principales:**

##### `bool compute(String& input, bool trace, std::ostream& os)`
**Bucle principal de simulación**. Coordina toda la ejecución:
1. Determina número de cintas: `model_.determineTapeCount()`
2. Inicializa cintas y cabezales: `initializeTapes()`
3. Obtiene estado inicial del modelo
4. **Bucle de ejecución**:
   - Verifica si estado actual es de aceptación → ACEPTA
   - Lee símbolos bajo todas las cabezas: `readCurrentSymbols()`
   - Busca transición aplicable: `findApplicableTransition()`
   - Si no hay transición → RECHAZA
   - Aplica transición: `applyTransition()`
   - Incrementa contador de pasos
   - Si supera MAX_STEPS → RECHAZA (evita bucles infinitos)
5. Aplana resultado: `flattenResult()` (copia cinta 0 a output)
6. Retorna `true` si aceptó, `false` si rechazó

**Parámetros**:
- `input`: String de entrada (se modifica para contener resultado final)
- `trace`: Activa modo traza detallado
- `os`: Stream de salida para la traza

##### `void initializeTapes(input, tapeCount, tapes, heads)`
**Inicialización de cintas**. Prepara el estado inicial:
- Crea `tapeCount` cintas vacías
- **Cinta 0**: carga la cadena de entrada símbolo por símbolo
- **Cintas 1..n**: inicializa con un símbolo blanco `.`
- Si entrada vacía: cinta 0 contiene solo `.`
- Inicializa todos los cabezales en posición 0

**Ejemplo**: Para entrada `"abb"` y 2 cintas:
```
Cinta 0: [a] b b
Cinta 1: [.]
```

##### `vector<Symbol> readCurrentSymbols(tapes, heads)`
**Lectura de símbolos actuales**. Lee qué hay bajo cada cabezal:
- Itera sobre todas las cintas
- Para cada cinta `t`, lee símbolo en posición `heads[t]`
- Si cabezal fuera de límites → retorna `.` (blanco)
- Retorna vector con un símbolo por cinta

**Crucial para multicinta**: Esta función es lo que permite comparar símbolos de **todas** las cintas simultáneamente.

##### `const Transition& findApplicableTransition(currentState, tapes, heads, found)`
**Búsqueda de transición aplicable**. Algoritmo de matching multicinta:

1. Lee símbolos actuales de todas las cintas: `readCurrentSymbols()`
2. Obtiene transiciones desde estado actual: `model_.getTransitionsFrom(currentState)` (búsqueda O(1) en map)
3. Para cada transición candidata:
   - Obtiene sus `readSymbols` (vector de símbolos esperados)
   - **Compara TODAS las cintas**: itera sobre `readSymbols` y `currentSymbols`
   - Si todos coinciden → transición aplicable ✓
   - Si alguno difiere → siguiente transición
4. Si encuentra match: establece `found = true`, retorna transición
5. Si no encuentra ninguna: establece `found = false`, retorna `emptyTransition_`

**Optimización**: Usa el map de transiciones del modelo para acceso directo mas eficiente en lugar de buscar en lista completa con un vector.

**Ejemplo de matching**:
```
Estado actual: q0
Símbolos leídos: [a, ., .]  (cinta0='a', cinta1='.', cinta2='.')
Transición: q0 [a, ., .] → q1
¿Match? ✓ Todos los símbolos coinciden
```

##### `void applyTransition(tr, tapes, heads, currentState)`
**Aplicación de transición**. Modifica el estado de la máquina:

1. **Escritura**: Para cada cinta en `tr.getTapeActions()`:
   - Escribe símbolo especificado en posición del cabezal
   - `tapes[i][heads[i]] = writeSymbol`

2. **Movimiento de cabezales**: Para cada cinta:
   - **LEFT (L)**:
     - Si cabezal en posición 0: expande cinta por la izquierda (inserta `.` al inicio)
     - Si no: decrementa posición `heads[i]--`
   - **RIGHT (R)**:
     - Incrementa posición `heads[i]++`
     - Si alcanza el final: expande cinta por la derecha (añade `.`)
   - **STAY (S)**: No hace nada

3. **Cambio de estado**: Obtiene estado destino del modelo y actualiza `currentState`
   - Usa `model_.getStateById()` para preservar flags de aceptación (búsqueda en el map)

**Ejemplo de aplicación**:
```
Transición: q0 [a,.] → q1 [(X,R), (.,S)]
Antes:  Cinta0: [a]bb  Cinta1: [.]
Escribe: X en cinta0, . en cinta1
Mueve:   R en cinta0, S en cinta1
Después: Cinta0: X[b]b  Cinta1: [.]
Estado: q0 → q1
```

##### `void flattenResult(input, tapes)`
**Aplanamiento de resultado**. Extrae el output final:
- Copia el contenido de **cinta 0** al objeto `String` de salida
- Permite que el llamador acceda al resultado final
- Solo cinta 0 se retorna (las demás son auxiliares)

**Nota**: Si se quisieran ver todas las cintas, modificar esta la opcion `--trace` que muestra un output mucho mas completo.

---

**Características de seguridad**:
- Expansión automática de cintas (crecen según necesidad)
- Protección contra bucles infinitos (MAX_STEPS)
- Validación de estados destino (lanza excepción si no existen)
- Validación de la i tegridad de las transiciones, tienen acciones para todas las cintas 
- Manejo de cabezales fuera de límites (retorna blanco)

#### **Transition** (`transition.h/cc`)
**Propósito**: Representa una transición multicinta.

**Estructura**:
```cpp
class Transition {
  State from_, to_;
  vector<Symbol> readSymbols_;     // Símbolos a leer (uno por cinta)
  map<int, pair<Symbol, Moves>> actions_;  // {cinta_id -> (símbolo, movimiento)}
};
```

**Cambio principal**: Pasó de `Symbol readSymbol` (singular) a `vector<Symbol> readSymbols` para soportar lectura multicinta.

#### **FileParser** (`file_parser.h/cc`)
**Propósito**: Parsea archivos de definición de MT.

**Proceso**:
1. Lee 6 líneas de cabecera (estados, alfabetos, estado inicial, blanco, aceptación)
2. `parseTransitionLines()`: lee transiciones en formato intercalado
3. Construye objetos `Transition` con todos los `readSymbols`
4. Retorna `TuringMachineModel` completo

**Complejidad del parseo**: El formato intercalado requiere leer primero todos los símbolos de lectura, luego todos los pares (write, move).

#### **State** (`state.h/cc`)
**Propósito**: Representa un estado con id y bandera de aceptación.

Simple pero esencial: `State(string id, bool accept)`. Se usa en maps para búsqueda rápida.

#### **Symbol** (`symbol.h/cc`)
**Propósito**: Sigue el patron Wrapper sobre `char` con operadores de comparación.

Permite usar `Symbol` en contenedores STL y simplifica el código del simulador.

#### **TracePrinter** (`trace_printer.h/cc`)
**Propósito**: Formatea salida visual paso a paso.

Genera tablas con bordes Unicode mostrando:
- Estado actual y símbolos leídos
- Transición aplicada
- Acciones por cinta (qué escribe, hacia dónde mueve)
- Contenido de cada cinta con cabezal marcado `[símbolo]`

#### **ArgParser** (`arg_parser.h/cc`)
**Propósito**: Parsea argumentos de línea de comandos.

Detecta flags `--trace` y `--info`, valida número de argumentos, muestra ayuda si falta información.

### Flujo de Ejecución

1. `main.cc`: parsea argumentos → carga MT con `FileParser` → crea `TuringMachineSimulator`
2. Para cada cadena: inicializa cintas → llama `simulator.simulate()`
3. `simulate()`: bucle hasta estado aceptación/rechazo/límite
4. En cada paso: `findApplicableTransition()` compara **todas las cintas** → `applyTransition()` modifica estado
5. Resultado → `FileOut.txt` + consola (si `--trace`)

## Modos de Ejecución

### Modo Normal
```bash
./pract-02 Inputs/MT/AnBn_Plus_MT.txt Inputs/Strigs/strings_anbn_plus.txt
```
Salida en `FileOut.txt`:
```
abb: ACEPTADA -> Resultado: .XYb
ab: RECHAZADA -> Resultado: .XY.
```

### Modo Trace (--trace)
Muestra ejecución paso a paso con tablas visuales:
```bash
./pract-02 Test/MT/Copy_2Tapes_MT.txt Test/Strings/strings_copy.txt --trace
```
- Estado actual y símbolos leídos por cinta
- Transición aplicada
- Acciones por cinta (símbolo escrito, movimiento)
- Contenido de todas las cintas con cabezal marcado `[símbolo]`

### Modo Info (--info)
Muestra estructura completa de la MT antes de ejecutar:
```bash
./pract-02 Inputs/MT/CountAB_2Tapes_MT.txt Inputs/Strigs/strings_ab.txt --info
```
- Estados (marcando aceptación)
- Alfabetos de entrada y cinta
- Todas las transiciones con formato: `Transition(q0 -> q1, read: [a, .], actions: {tape0:(X,R), tape1:(.,S)})`

Combinar ambos: `./pract-02 <MT> <strings> --info --trace`

## Resolución de Problemas

### Ejercicio 1: a^n b^m donde m > n

**Problema**: Diseñar una MT que acepte cadenas del lenguaje L = {a^n b^m | m > n, n ≥ 1}

**Ejemplos**:
- Acepta: `abb`, `abbb`, `aabbb`, `aaaabbbbbb`
- Rechaza: `ab` (m no es > n), `aabb` (m = n), `aaabbb` (m = n), `ba`, `ε`

**Estrategia de solución**:
1. **Marcado por parejas**: Por cada 'a', marcar una 'b' con 'Y'
2. **Verificación final**: Al terminar de marcar todas las 'a's, debe quedar al menos una 'b' sin marcar
3. **Uso de símbolos**: 'X' marca las 'a's procesadas, 'Y' marca las 'b's emparejadas

**Algoritmo (1 cinta)**:
```
Estado q0: Buscar siguiente 'a' no marcada
  - Si encuentra 'a': marcarla con 'X', ir a q1
  - Si encuentra 'X': avanzar (ya procesada)
  - Si encuentra 'Y': todas las 'a's procesadas, ir a q3 (verificación)

Estado q1: Buscar 'b' no marcada para emparejar
  - Avanzar sobre 'a's y 'Y's (ya marcadas)
  - Al encontrar 'b': marcarla con 'Y', volver a q2

Estado q2: Regresar al inicio
  - Retroceder hasta el comienzo
  - Ir a q0 para procesar siguiente 'a'

Estado q3: Verificación final
  - Debe existir al menos una 'b' sin marcar
  - Si encuentra 'b': ACEPTAR
  - Si no hay 'b's: RECHAZAR
```

**Archivo**: `Inputs/MT/AnBn_Plus_MT.txt`

**Transiciones clave**:
```
q0 a q1 X R      # Marcar 'a' con 'X', buscar 'b'
q1 b q2 Y L      # Marcar 'b' con 'Y', volver
q2 . q0 . R      # Inicio alcanzado, procesar siguiente 'a'
q0 Y q3 Y R      # Todas las 'a's procesadas, verificar
q3 b qaccept b S # Existe 'b' sin marcar → ACEPTAR
```
---

### Ejercicio 2: Contador de a's y b's

#### Primera Solucion primero a's ,despues b's

**Problema**: Diseñar una MT de 3 cintas que cuente 'a's y 'b's y produzca resultado en formato unario: `.1^(n_a+1).1^(n_b+1).`

**Ejemplos**:
- `a` → `.11.1.` (1 'a' → dos 1's, 0 'b's → un 1)
- `aa` → `.111.1.` (2 'a's → tres 1's, 0 'b's → un 1)
- `abb` → `.11.111.` (1 'a' → dos 1's, 2 'b's → tres 1's)
- `b` → `.1.11.` (0 'a's → un 1, 1 'b' → dos 1's)

**Estrategia de solución**:
1. **Contar en cinta auxiliar**: Usar cinta 1 como contador temporal
2. **Copiar a cinta 0**: Transferir resultado en formato unario
3. **Separador de puntos**: Usar '.' para delimitar contadores

**Algoritmo (3 cintas)**:
```
Fase 1: Procesar 'a's (estados q0-q4)
  - Por cada 'a': escribir '1' en cinta 1
  - Borrar 'a' de cinta 0
  - Al terminar 'a's: escribir separador '.'

Fase 2: Copiar contador de 'a's a cinta 0
  - Copiar de cinta 1 a cinta 0
  - Añadir un '1' extra (n_a + 1)
  - Escribir separador '.'

Fase 3: Procesar 'b's (estados q5-q7)
  - Por cada 'b': escribir '1' en cinta 1
  - Borrar 'b' de cinta 0
  - Copiar contador de 'b's a cinta 0
  - Añadir un '1' extra (n_b + 1)
  - Escribir '.' final

Caso especial: Solo 'a's sin 'b's (estado q8-q9)
  - Manejar cadenas tipo "aa" o "a"
  - Escribir ".1^(n_a+1).1." (contador de b's = 0 + 1)
```

**Archivo**: `Inputs/MT/CountAB_2Tapes_MT.txt` (usa 3 cintas)

**Transiciones clave**:
```
# Contar 'a's
q0 a q1 . R . 1 R . . S    # Por cada 'a': escribir 1 en cinta1, borrar en cinta0

# Copiar contador de 'a's con +1
q3 . q3 1 L . . S 1 . L    # Escribir 1's en cinta0 y cinta2
q4 . q4 1 L 1 . L . . S    # Copiar y agregar +1

# Contar 'b's
q5 b q5 . R . . S . 1 R    # Por cada 'b': escribir 1 en cinta2

# Copiar contador de 'b's con +1
q6 . q6 1 L . . S 1 . L    # Escribir 1's en cinta0
q7 . q7 1 L 1 . L . . S    # Copiar y agregar +1
```

**Resultado**: Cinta 0 contiene `.1^(n_a+1).1^(n_b+1).` donde n_a y n_b son los conteos.

#### Segunda Solucion da igual el orden de a's y b's

**Problema**: Diseñar una MT de 3 cintas que cuente 'a's y 'b's siendo que las a's y las b's pueden estar intercaladas y produzca resultado en formato unario: `.1^(n_a+1).1^(n_b+1).`

**Ejemplos**:
- `a` → `.11.1.` (1 'a' → dos 1's, 0 'b's → un 1)
- `aa` → `.111.1.` (2 'a's → tres 1's, 0 'b's → un 1)
- `abb` → `.11.111.` (1 'a' → dos 1's, 2 'b's → tres 1's)
- `b` → `.1.11.` (0 'a's → un 1, 1 'b' → dos 1's)
- `ba` → `.11.11.` (1 'a' → dos 1's, 1 'b's → dos 1's)
- `bbaa` → `.111.111.` (2 'a's → tres 1's, 2 'b's → tres 1's)
- `babab` → `.111.1111.` (2 'a' → tres 1's, 3 'b's → cuatro 1's)
- `bab` → `.11.111.` (1 'a's → dos 1's, 2 'b's → tres 1's)

**Estrategia de solución**:
1. **Contar en las cintas auxiliares**: Borrar las a's y b's y usar cinta 1 como contador temporal de a's y la 2 como contador de b's
2. **Copiar la cinta 1**: poner los 1's de la cinta uno en la cinta inicial
3. **Copiar la cinta 2**: poner los 1's de la cinta dos en la cinta inicial

**Algoritmo (3 cintas)**:
```

# Transiciones
# q0: Copiar a's a cinta 1 y b's a la cinta 2 y el 1 de mas de las a
q0 b q0 . R . . S . 1 R
q0 a q0 . R . 1 R . . S
q0 . q1 1 R . . L . . S

# q1: poner todos los 1 de las a y el blanco entre medias
q1 . q1 1 R 1 . L . . S
q1 . q2 . R . . S . . L

# q2: poner todos los 1 de b y el uno de mas de las b
q2 . q2 1 R . . S 1 . L
q2 . qaccept 1 R . . S . . S
```

**Archivo**: `Inputs/MT/CountAB_2Tapes_MT_Random.txt` (usa 3 cintas)

**Transiciones clave**:
```
# añade el 1 extra de las a's
q0 . q1 1 R . . L . . S


# añade el blanco entre la cuenta de a's y b's
q1 . q2 . R . . S . . L

# añade el 1 extra de las b's
q2 . qaccept 1 R . . S . . S
```

**Resultado**: Cinta 0 contiene `.1^(n_a+1).1^(n_b+1).` donde n_a y n_b son los conteos.

---

## Testing

El proyecto incluye 11 máquinas de Turing:
- **Inputs/MT/**: 2 ejemplos principales (AnBn_Plus, CountAB)
- **Test/MT/**: 9 MTs de prueba (EvenAs, AStarBStar, BinaryReverse, Copy, Palindrome, etc.)

```bash
./test_all.sh  # Ejecuta todos los tests, muestra resultados esperados, guarda en Test/Outputs/
```

Cada test documenta qué cadenas deberían ser aceptadas/rechazadas. Resultados en `Test/Outputs/*.out.results`.

## Ejemplos de Uso

```bash
# Ejemplo 1: a^n b^m donde m > n
./pract-02 Inputs/MT/AnBn_Plus_MT.txt Inputs/Strigs/strings_anbn_plus.txt

# Ejemplo 2: Contador de a's y b's (3 cintas)
./pract-02 Inputs/MT/CountAB_2Tapes_MT.txt Inputs/Strigs/strings_ab.txt --trace

# Test: Copia entre cintas (2 cintas)
./pract-02 Test/MT/Copy_2Tapes_MT.txt Test/Strings/strings_copy.txt --info

# Test: Palíndromos (3 cintas)
./pract-02 Test/MT/Palindrome_3Tapes_MT.txt Test/Strings/strings_palindrome.txt --trace
```

---

**Autor**: Kyliam Chinea Salcedo
**Asignatura**: Complejidad y Computacional
