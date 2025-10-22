# Práctica 2 - Simulador de Máquinas de Turing Multicinta

**Asignatura:** Complejidad Computacional  
**Autor:** Kyliam Gabriel Chinea Salcedo  
**Curso:** 2024-2025

---

## 📋 Índice

1. [Introducción](#-introducción)
2. [Formato de Inputs](#-formato-de-inputs)
   - [Formato del fichero de definición de MT](#formato-del-fichero-de-definición-de-mt)
   - [Formato del fichero de cadenas](#formato-del-fichero-de-cadenas)
3. [Ejercicios de Máquinas de Turing](#-ejercicios-de-máquinas-de-turing)
   - [Ejercicio 1: Lenguaje a^n b^m (m > n, n ≥ 1)](#ejercicio-1-lenguaje-an-bm-m--n-n--1)
   - [Ejercicio 2: [Por completar]](#ejercicio-2-por-completar)
4. [Arquitectura del Código](#-arquitectura-del-código)
   - [Diagrama de Clases](#diagrama-de-clases)
   - [Descripción Detallada de Clases](#descripción-detallada-de-clases)
5. [Compilación y Ejecución](#-compilación-y-ejecución)
6. [Ejemplos de Uso](#-ejemplos-de-uso)

---

## 🎯 Introducción

Este proyecto implementa un **simulador de Máquinas de Turing multicinta** en C++11. El simulador es capaz de:

- ✅ Parsear definiciones de MT desde ficheros de texto
- ✅ Ejecutar simulaciones sobre múltiples cadenas de entrada
- ✅ Soportar transiciones multicinta (múltiples cabezas lectoras/escritoras)
- ✅ Proporcionar un modo de traza detallado para debugging
- ✅ Generar resultados en formato estructurado

El proyecto sigue principios de **diseño orientado a objetos** con separación de responsabilidades, encapsulación y uso de referencias en lugar de punteros para mayor seguridad.

---

## 📄 Formato de Inputs

### Formato del fichero de definición de MT

El fichero de definición de una Máquina de Turing consta de **7 secciones**, una por línea:

```
<estados>
<alfabeto_entrada>
<alfabeto_cinta>
<estado_inicial>
<simbolo_blanco>
<estados_aceptacion>
<transiciones>
```

#### Descripción de cada sección:

1. **Estados**: Lista de estados separados por espacios
   ```
   q0 q1 q2 qaccept qreject
   ```

2. **Alfabeto de entrada**: Símbolos del alfabeto de entrada (Σ)
   ```
   a b
   ```

3. **Alfabeto de cinta**: Símbolos del alfabeto de cinta (Γ), debe incluir el blanco
   ```
   a b X Y .
   ```

4. **Estado inicial**: Estado donde comienza la ejecución
   ```
   q0
   ```

5. **Símbolo blanco**: Símbolo que representa celdas vacías (normalmente `.`)
   ```
   .
   ```

6. **Estados de aceptación**: Estados que aceptan la cadena (separados por espacios)
   ```
   qaccept
   ```

7. **Transiciones**: Una transición por línea con formato:
   ```
   <estado_origen> <símbolo_leído> <estado_destino> <símbolo_escrito_cinta0> <movimiento_cinta0> [<símbolo_escrito_cinta1> <movimiento_cinta1> ...]
   ```

   **Movimientos:**
   - `L` - Izquierda (Left)
   - `R` - Derecha (Right)
   - `S` - Sin movimiento (Stay)

#### Ejemplo completo:

```
q0 q1 qaccept
a b
a b .
q0
.
qaccept
q0 a q1 . R
q1 b qaccept . R
```

### Formato del fichero de cadenas

Fichero de texto plano donde **cada línea contiene una cadena de entrada** a procesar:

```
aa
ab
ba
aabb
```

**Nota:** Las líneas vacías son ignoradas automáticamente.

---

## 🧮 Ejercicios de Máquinas de Turing

### Ejercicio 1: Lenguaje a^n b^m (m > n, n ≥ 1)

**Objetivo:** Diseñar una MT que reconozca el lenguaje L = {a^n b^m | m > n, n ≥ 1}

**Descripción:**
- La cadena debe tener **al menos una 'a'** seguida de **al menos dos 'b'**
- Debe haber **estrictamente más 'b's que 'a's**
- Ejemplos válidos: `abb`, `abbb`, `aabbb`, `aaabbbb`
- Ejemplos inválidos: `ab`, `aabb`, `aaabbb`, `a`, `b`

**Estrategia de la MT:**

1. **Fase de marcado:** Marca cada 'a' con 'X' y cada 'b' con 'Y'
2. **Fase de verificación:** Cuenta que haya al menos una 'b' sin marcar al final
3. **Aceptación:** Si quedan 'b's sin marcar y todas las 'a's están marcadas

**Diagrama de Estados:**

```
                    ┌─────────────────────────────────────┐
                    │                                     │
                    ▼                                     │
    ┌────┐   a/X,R  ┌────┐   Y/Y,R    ┌────┐   b/Y,L     ┌──── ┐
───►│ q0 │─────────►│ q1 │───────────►│ q1 │◄────────────│ q2  │
    └────┘          └──┬─┘            └─┬──┘             └── ┬─┘
                       │                 │                   │
                       │ b/Y,R           │ ./.,R             │ X/X,L
                       │                 │                   │
                       │                 │                   │
                       │              ┌──▼──┐                │
                       └─────────────►│qaccept               │
                                      └─────┘                │
                                         ▲                   │
                                         └───────────────────┘
                                              a/X,R
    
    Estados:
    • q0: Estado inicial, busca primera 'a'
    • q1: Avanza buscando una 'b' para emparejar
    • q2: Retrocede al inicio para siguiente ciclo
    • qaccept: Acepta si hay 'b's extra
    
    Símbolos:
    • a, b: Símbolos originales
    • X: 'a' marcada (procesada)
    • Y: 'b' marcada (procesada)
    • .: Blanco
```

**Archivo:** `Inputs/MT/AnBn_Plus_MT.txt`

**Tabla completa de transiciones:**

| Estado | Símbolo leído | Estado destino | Símbolo escrito | Movimiento |
|--------|---------------|----------------|-----------------|------------|
| q0     | a             | q1             | X               | R          |
| q0     | X             | q0             | X               | R          |
| q0     | Y             | q3             | Y               | R          |
| q1     | a             | q1             | a               | R          |
| q1     | Y             | q1             | Y               | R          |
| q1     | b             | q2             | Y               | L          |
| q2     | a             | q2             | a               | L          |
| q2     | Y             | q2             | Y               | L          |
| q2     | X             | q2             | X               | L          |
| q2     | .             | q0             | .               | R          |
| q3     | Y             | q3             | Y               | R          |
| q3     | b             | qaccept        | b               | S          |

**Explicación detallada:**

1. **Estado q0 (Búsqueda de 'a' no marcada):**
   - `q0 --a/X,R--> q1`: Encuentra 'a', la marca como 'X' y busca su 'b'
   - `q0 --X/X,R--> q0`: Salta 'X's (a's ya procesadas)
   - `q0 --Y/Y,R--> q3`: Solo quedan 'Y's, verifica que haya 'b's sin marcar

2. **Estado q1 (Búsqueda de 'b' para emparejar):**
   - `q1 --a/a,R--> q1`: Salta 'a's no procesadas
   - `q1 --Y/Y,R--> q1`: Salta 'Y's (b's ya emparejadas)
   - `q1 --b/Y,L--> q2`: Encuentra 'b', la marca como 'Y' y retrocede

3. **Estado q2 (Retroceso al inicio):**
   - `q2 --a/a,L--> q2`: Retrocede sobre 'a's
   - `q2 --Y/Y,L--> q2`: Retrocede sobre 'Y's
   - `q2 --X/X,L--> q2`: Retrocede sobre 'X's
   - `q2 --./.,R--> q0`: Alcanza blanco inicial, reinicia ciclo

4. **Estado q3 (Verificación de 'b's extras):**
   - `q3 --Y/Y,R--> q3`: Avanza sobre 'Y's buscando 'b' sin marcar
   - `q3 --b/b,S--> qaccept`: ¡Encuentra 'b' extra! → ACEPTA (m > n)

**Transiciones (formato compacto):**
```
# Estado q0: Buscar 'a' no marcada
q0 a q1 X R    # Marca 'a' como 'X'
q0 X q0 X R    # Salta 'X's procesadas
q0 Y q3 Y R    # Todas las 'a's procesadas, verifica extras

# Estado q1: Buscar 'b' para emparejar
q1 a q1 a R    # Avanza sobre 'a's
q1 Y q1 Y R    # Avanza sobre 'Y's
q1 b q2 Y L    # Marca 'b', retrocede

# Estado q2: Retroceder al inicio
q2 a q2 a L    # Retrocede sobre 'a'
q2 Y q2 Y L    # Retrocede sobre 'Y'
q2 X q2 X L    # Retrocede sobre 'X'
q2 . q0 . R    # Vuelve al inicio

# Estado q3: Verificar 'b's extras
q3 Y q3 Y R    # Salta 'Y's
q3 b qaccept b S  # ¡'b' extra! → ACEPTA
```

**Cadenas de prueba:**
```bash
./pract-02 Inputs/MT/AnBn_Plus_MT.txt Inputs/Strigs/strings_anbn_plus.txt
```

**Resultados esperados:**
- ✅ `abb` → ACEPTADA
- ✅ `abbb` → ACEPTADA
- ✅ `aabbb` → ACEPTADA
- ❌ `ab` → RECHAZADA
- ❌ `aabb` → RECHAZADA
- ❌ `aaabbb` → RECHAZADA

---

### Ejercicio 2: Contador de 'a's y 'b's en Formato Unario (3 cintas)

**Objetivo:** Contar el número de 'a's y 'b's en la cadena de entrada y escribir el resultado en formato unario.

**Descripción:**
Dada una cadena de entrada w ∈ {a, b}*, la máquina debe transformarla en:
```
.1^(n_a+1).1^(n_b+1).
```
donde:
- n_a = número de 'a's en la cadena
- n_b = número de 'b's en la cadena
- Cada grupo de unos representa el conteo + 1 (mínimo)

**Ejemplos:**
- `a` → `.11.1.` (1 'a' → 11, 0 'b's → 1)
- `aa` → `.111.1.` (2 'a's → 111, 0 'b's → 1)
- `b` → `.1.11.` (0 'a's → 1, 1 'b' → 11)
- `abb` → `.11.111.` (1 'a' → 11, 2 'b's → 111)

**Estrategia de la MT (3 cintas):**

**Cintas:**
- **Cinta 0**: Entrada/Salida - se transforma progresivamente
- **Cinta 1**: Cinta auxiliar para contar (1's temporales)
- **Cinta 2**: Cinta auxiliar adicional (sin uso activo)

**Fases:**

1. **Fase 1: Contar 'a's**
   - Recorrer cinta 0 buscando 'a's
   - Por cada 'a' encontrada: borrarla de cinta 0, escribir '1' en cinta 1
   - Las 'b's se preservan para la siguiente fase

2. **Fase 2: Escribir resultado de 'a's en cinta 0**
   - Agregar '1' inicial (mínimo)
   - Leer '1's de cinta 1 y escribirlos en cinta 0
   - Escribir punto separador '.'

3. **Fase 3: Contar 'b's**
   - Recorrer cinta 0 buscando 'b's
   - Por cada 'b' encontrada: borrarla, escribir '1' en cinta 1

4. **Fase 4: Escribir resultado de 'b's**
   - Agregar '1' inicial (mínimo)
   - Copiar '1's de cinta 1 a cinta 0
   - Finalizar en estado de aceptación

**Diagrama de Estados:**
```
                    a/.,R
    ┌────┐       ┌────────┐  a/.,R       ┌────┐
───►│ q0 │──────►│   q1   │─────────────►│ q1 │ (Contar a's)
    └──┬─┘       └────┬───┘              └──┬─┘
       │              │ b                   │ .
       │ b            ▼                     ▼
       │          ┌────┐                ┌────┐
       └─────────►│ q2 │───────────────►│ q3 │ (Escribir resultado a's)
                  └────┘      .          └──┬─┘
                                            │
                                            ▼
                  ┌────┐                ┌────┐
                  │ q5 │◄───────────────│ q4 │ (Agregar 1 extra a's)
                  └──┬─┘                └────┘
                     │ b/.,R
                     ▼
                  ┌────┐                ┌────┐
                  │ q6 │───────────────►│ q7 │ (Escribir resultado b's)
                  └────┘      .          └──┬─┘
                                            │
                                            ▼
                                        ┌────────┐
                                        │qaccept │
                                        └────────┘
```

**Archivo:** `Inputs/MT/CountAB_2Tapes_MT.txt` *(nota: nombre de archivo mantiene "2Tapes" pero usa 3)*

**Formato de transiciones (3 cintas):**
```
<estado> <lec0> <sig_estado> <esc0> <mov0> <lec1> <esc1> <mov1> <lec2> <esc2> <mov2>
```

**Transiciones principales:**
```
# Contar a's (borrar de cinta 0, escribir en cinta 1)
q0 a q1 . R . 1 R . . S    # Lee 'a', borra, escribe '1' en cinta 1
q1 a q1 . R . 1 R . . S    # Continúa contando 'a's

# Encontrar b o fin de a's
q1 b q2 b S . . S . . S    # Encontró 'b', preparar fase de escritura
q1 . q8 1 L . . S . . S    # No hay b's, ir a escribir solo a's

# Escribir resultado de a's
q3 . q3 1 L . . S 1 . L    # Escribe '1's en cinta 0
q4 . q4 1 L 1 . L . . S    # Agrega '1' extra para a's

# Contar b's
q5 b q5 . R . . S . 1 R    # Borra 'b', escribe '1' en cinta 2
q6 . q6 1 L . . S 1 . L    # Escribe '1's para b's

# Agregar 1 extra para b's
q7 . q7 1 L 1 . L . . S    # Agrega '1' extra
q7 . qaccept . L . . L . . L  # Acepta
```

**Cadenas de prueba:**
```bash
./pract-02 Inputs/MT/CountAB_2Tapes_MT.txt Inputs/Strigs/strings_ab.txt
```

**Resultados esperados:**
- ✅ `a` → `.11.1.` (1 'a', 0 'b's)
- ✅ `aa` → `.111.1.` (2 'a's, 0 'b's)
- ✅ `abb` → `.11.111.` (1 'a', 2 'b's)
- ✅ `b` → `.1.11.` (0 'a's, 1 'b')

---

### Ejemplos Adicionales de Máquinas de Turing

#### Ejemplo 1: Número Par de 'a's (1 cinta)

**Lenguaje:** L = {w ∈ {a,b}* | w contiene un número par de 'a's}

**Diagrama de Estados:**

```
              a/a,R
        ┌───────────────┐
        │               │
        │               ▼
    ┌────┐   b/b,R   ┌────┐   ./.,S   ┌────────┐
───►│ q0 │───────────►│ q0 │───────────►│qaccept │
    └────┘            └──┬─┘            └────────┘
      ▲                  │
      │                  │ a/a,R
      │                  ▼
      │   b/b,R       ┌────┐
      └───────────────│ q1 │
                      └────┘
    
    Estados:
    • q0: Número par de 'a's vistas (estado aceptador)
    • q1: Número impar de 'a's vistas
    • qaccept: Estado final de aceptación
    
    Transiciones:
    • Desde q0: 'a' → q1 (par → impar)
    • Desde q1: 'a' → q0 (impar → par)
    • Las 'b's no cambian el estado (autobucle)
```

**Archivo:** `Inputs/MT/EvenAs_1Tape_MT.txt`

**Tabla completa de transiciones:**

| Estado | Símbolo leído | Estado destino | Símbolo escrito | Movimiento |
|--------|---------------|----------------|-----------------|------------|
| q0     | a             | q1             | a               | R          |
| q0     | b             | q0             | b               | R          |
| q0     | .             | qaccept        | .               | S          |
| q1     | a             | q0             | a               | R          |
| q1     | b             | q1             | b               | R          |
| q1     | .             | q1             | .               | S          |

**Explicación:**
- **q0 (par)**: Al leer 'a' va a q1 (impar), al leer 'b' permanece en q0, al leer '.' acepta
- **q1 (impar)**: Al leer 'a' va a q0 (par), al leer 'b' permanece en q1, al leer '.' rechaza (autobucle infinito)

**Transiciones:**
```
q0 a q1 a R      # Par → Impar
q0 b q0 b R      # Salta 'b's sin cambiar paridad
q0 . qaccept . S # Blanco en estado par → ACEPTA
q1 a q0 a R      # Impar → Par
q1 b q1 b R      # Salta 'b's sin cambiar paridad
q1 . q1 . S      # Blanco en estado impar → Bucle infinito (RECHAZA)
```

**Ejemplos:**
- ✅ `aa`, `aaaa`, `bb`, `aabb`, `abab` → ACEPTADAS
- ❌ `a`, `aaa`, `ababab` → RECHAZADAS

---

#### Ejemplo 2: Lenguaje a*b* (1 cinta)

**Lenguaje:** L = {a^n b^m | n,m ≥ 0}

**Diagrama de Estados:**

```
              a/a,R
        ┌───────────────┐
        │               │
        │               ▼
    ┌────┐            ┌────┐   b/b,R    ┌────┐   ./.,S    ┌────────┐
───►│ q0 │            │ q0 │───────────►│ q1 │───────────►│qaccept │
    └──┬─┘            └────┘            └──┬─┘            └────────┘
       │                                   │
       │ ./.,S                             │ b/b,R
       │                                   │
       │                                   │
       └───────────────────────────────────┘
    
    Estados:
    • q0: Leyendo 'a's (acepta ε también)
    • q1: Leyendo 'b's (no puede volver a 'a's)
    • qaccept: Aceptación
    
    Funcionamiento:
    • En q0: lee todas las 'a's
    • Al encontrar 'b': transiciona a q1
    • En q1: solo acepta 'b's y blanco
    • Si encuentra 'a' después de 'b': RECHAZA (no hay transición)
```

**Archivo:** `Inputs/MT/AStarBStar_1Tape_MT.txt`

**Tabla completa de transiciones:**

| Estado | Símbolo leído | Estado destino | Símbolo escrito | Movimiento |
|--------|---------------|----------------|-----------------|------------|
| q0     | a             | q0             | a               | R          |
| q0     | b             | q1             | b               | R          |
| q0     | .             | qaccept        | .               | S          |
| q1     | b             | q1             | b               | R          |
| q1     | .             | qaccept        | .               | S          |

**Explicación:**
- **q0**: Acepta 'a's y permanece en q0, al ver 'b' pasa a q1, al ver '.' acepta (cadena vacía o solo a's)
- **q1**: Solo acepta 'b's y blanco. No hay transición para 'a', por lo que rechaza si aparece 'a' después de 'b'

**Transiciones:**
```
q0 a q0 a R      # Lee 'a's consecutivas
q0 b q1 b R      # Primera 'b' → cambia a modo "solo b's"
q0 . qaccept . S # Acepta cadena vacía o solo a's
q1 b q1 b R      # Lee 'b's consecutivas
q1 . qaccept . S # Acepta al terminar con b's
# NO HAY: q1 a ... → Por tanto, rechaza si hay 'a' después de 'b'
```

**Ejemplos:**
- ✅ `a`, `b`, `aa`, `bb`, `ab`, `aabb`, `aaabbb` → ACEPTADAS
- ❌ `ba`, `aba`, `abba` → RECHAZADAS

---

#### Ejemplo 3: Reversión Binaria (2 cintas)

**Lenguaje:** Copia la cadena binaria y la invierte en la cinta 2

**Diagrama de Estados (simplificado):**

```
                    0/0,R; 0,R
                    1/1,R; 1,R
        ┌──────────────────────────┐
        │                          │
        │                          ▼
    ┌────┐                      ┌────┐   ./.,L; .,L   ┌────┐
───►│ q0 │                      │ q0 │───────────────►│ q1 │
    └────┘                      └────┘                └──┬─┘
                                                         │
                                                         │ 0/.,L; 0,L
                    ┌────────────────────────────────────┤ 1/.,L; 1,L
                    │                                    │
                    │                                    ▼
                 ┌────┐                              ┌────┐   ./.,S; .,S   ┌────────┐
                 │qaccept◄──────────────────────────│ q1 │───────────────►│qaccept │
                 └────┘                              └────┘                └────────┘
    
    Funcionamiento (2 cintas):
    • Cinta 0: Lee la entrada
    • Cinta 1: Escribe la reversión
    
    Fases:
    1. q0: Copia símbolos a ambas cintas, avanza →
    2. q1: Retrocede en ambas cintas ←, borra cinta 0
    3. q2: Avanza borrando cinta 1
    4. qaccept: Acepta cuando termina
```

**Archivo:** `Inputs/MT/BinaryReverse_2Tapes_MT.txt`

**Tabla completa de transiciones (2 cintas):**

| Estado | Símbolo (cinta 0) | Estado destino | Cinta 0: Escribe/Mueve | Cinta 1: Escribe/Mueve |
|--------|-------------------|----------------|------------------------|------------------------|
| q0     | 0                 | q0             | 0, R                   | 0, R                   |
| q0     | 1                 | q0             | 1, R                   | 1, R                   |
| q0     | .                 | q1             | ., L                   | ., L                   |
| q1     | 0                 | q1             | ., L                   | 0, L                   |
| q1     | 1                 | q1             | ., L                   | 1, L                   |
| q1     | .                 | q2             | ., R                   | ., R                   |
| q2     | 0                 | q2             | 0, R                   | ., R                   |
| q2     | 1                 | q2             | 1, R                   | ., R                   |
| q2     | .                 | qaccept        | ., S                   | ., S                   |

**Explicación por fases:**

1. **Fase q0 (Copia a ambas cintas):**
   - `q0 --0--> q0`: Escribe '0' en ambas cintas, avanza →→
   - `q0 --1--> q0`: Escribe '1' en ambas cintas, avanza →→
   - `q0 --.--> q1`: Termina copia, retrocede ambas cintas

2. **Fase q1 (Retroceso y borrado cinta 0):**
   - `q1 --0--> q1`: Borra cinta 0 (.), mantiene '0' en cinta 1, retrocede ←←
   - `q1 --1--> q1`: Borra cinta 0 (.), mantiene '1' en cinta 1, retrocede ←←
   - `q1 --.--> q2`: Alcanza inicio, avanza para limpiar cinta 1

3. **Fase q2 (Limpiar cinta 1):**
   - `q2 --0--> q2`: Mantiene cinta 0, borra cinta 1, avanza →→
   - `q2 --1--> q2`: Mantiene cinta 0, borra cinta 1, avanza →→
   - `q2 --.--> qaccept`: Termina limpieza, ACEPTA

**Transiciones (formato multicinta):**
```
# Fase 1: Copiar a ambas cintas
q0 0 q0 0 R 0 R    # Copia '0' a cinta 0 y 1
q0 1 q0 1 R 1 R    # Copia '1' a cinta 0 y 1
q0 . q1 . L . L    # Fin copia, retrocede

# Fase 2: Borrar cinta 0, mantener cinta 1
q1 0 q1 . L 0 L    # Borra '0' en cinta 0
q1 1 q1 . L 1 L    # Borra '1' en cinta 0
q1 . q2 . R . R    # Vuelve al inicio

# Fase 3: Borrar cinta 1
q2 0 q2 0 R . R    # Mantiene cinta 0, borra cinta 1
q2 1 q2 1 R . R    # Mantiene cinta 0, borra cinta 1
q2 . qaccept . S . S # Termina → ACEPTA
```

**Ejemplos:**
- ✅ `0`, `1`, `10`, `101`, `1010` → ACEPTADAS (todas)

**Nota:** Este ejemplo muestra el formato multicinta: `estado símbolo_leído estado_dest escribe0 mov0 escribe1 mov1`

---

## 🏗️ Arquitectura del Código

El proyecto sigue el principio de **Separación de Responsabilidades (SRP)** con una arquitectura modular y limpia.

### Diagrama de Clases

```
                    ┌─────────────┐
                    │    main     │
                    └──────┬──────┘
                           │
              ┌────────────┼────────────┐
              │            │            │
         ┌────▼────┐  ┌────▼────┐ ┌────▼──────┐
         │  Args   │  │  File   │ │  Turing   │
         │         │  │ Parser  │ │  Machine  │
         └─────────┘  └────┬────┘ └─────┬─────┘
                           │            │
                      ┌────▼────┐       │
                      │ Symbol  │       │
                      │ State   │       │
                      │Alphabet │       │
                      │Transition│      │
                      └─────────┘       │
                                        │
                         ┌──────────────┴──────────────┐
                         │                             │
                    ┌────▼────────┐          ┌─────────▼────────┐
                    │   Turing    │          │     Turing       │
                    │   Machine   │◄─────────┤     Machine      │
                    │    Model    │          │    Simulator     │
                    └─────────────┘          └──────┬───────────┘
                                                    │
                                             ┌──────▼───────┐
                                             │    Trace     │
                                             │   Printer    │
                                             └──────────────┘
```

---

### Descripción Detallada de Clases

#### 1. **Args** (`arg_parser.h`, `arg_parser.cc`)

**Responsabilidad:** Parsear y almacenar argumentos de línea de comandos.

**Atributos privados:**
```cpp
std::string mtFile_;      // Ruta al fichero de definición de MT
std::string stringsFile_; // Ruta al fichero de cadenas
bool trace_;              // Flag para activar modo traza
```

**Métodos públicos:**
```cpp
Args(int argc, char* argv[]);              // Constructor que parsea argumentos
const std::string& getMtFile() const;      // Obtiene fichero de MT
const std::string& getStringsFile() const; // Obtiene fichero de cadenas
bool getTrace() const;                     // Obtiene flag de traza
```

**Ejemplo de uso:**
```cpp
Args args(argc, argv);
std::string mtFile = args.getMtFile();
bool traceMode = args.getTrace();
```

---

#### 2. **Symbol** (`symbol.h`, `symbol.cc`)

**Responsabilidad:** Representar un símbolo del alfabeto.

**Atributos privados:**
```cpp
char symbol_; // Carácter que representa el símbolo
```

**Métodos públicos:**
```cpp
Symbol(char c);                    // Constructor
char getSymbol() const;            // Obtiene el carácter
bool operator==(const Symbol& s);  // Comparación
bool operator<(const Symbol& s);   // Ordenación (para maps)
```

**Ejemplo de uso:**
```cpp
Symbol a('a');
Symbol blank('.');
if (a == blank) { /* ... */ }
```

---

#### 3. **State** (`state.h`, `state.cc`)

**Responsabilidad:** Representar un estado de la MT.

**Atributos privados:**
```cpp
std::string id_;     // Identificador del estado
bool isAccept_;      // ¿Es estado de aceptación?
```

**Métodos públicos:**
```cpp
State(const std::string& id);      // Constructor
const std::string& getId() const;  // Obtiene ID
bool isAccept() const;             // ¿Es aceptador?
void setAccept(bool accept);       // Marca como aceptador
```

**Ejemplo de uso:**
```cpp
State q0("q0");
State qaccept("qaccept");
qaccept.setAccept(true);
```

---

#### 4. **Alphabet** (`alphabet.h`, `alphabet.cc`)

**Responsabilidad:** Representar un alfabeto (conjunto de símbolos).

**Atributos privados:**
```cpp
std::vector<Symbol> symbols_; // Vector de símbolos
```

**Métodos públicos:**
```cpp
Alphabet(const std::vector<Symbol>& symbols); // Constructor
bool contains(const Symbol& s) const;         // ¿Contiene símbolo?
```

---

#### 5. **Transition** (`transition.h`, `transition.cc`)

**Responsabilidad:** Representar una transición de la MT.

**Atributos privados:**
```cpp
State from_;                                           // Estado origen
State to_;                                             // Estado destino
Symbol readSymbol_;                                    // Símbolo leído (cinta 0)
std::map<int, std::pair<Symbol, Moves>> tapeActions_; // Acciones por cinta
```

**Métodos públicos:**
```cpp
Transition(const State& from, const State& to, const Symbol& readSymbol,
           const std::map<int, std::pair<Symbol, Moves>>& tapeActions);
const State& getFrom() const;
const State& getTo() const;
const Symbol& getReadSymbol() const;
const std::map<int, std::pair<Symbol, Moves>>& getTapeActions() const;
```

**Estructura de datos:**
- `tapeActions_` es un `map<int, pair<Symbol, Moves>>`
- **Clave:** Índice de cinta (0, 1, 2, ...)
- **Valor:** Par (símbolo a escribir, movimiento)

**Ejemplo de uso:**
```cpp
// Transición: q0 --a--> q1, escribe 'X' en cinta 0, mueve RIGHT
std::map<int, std::pair<Symbol, Moves>> actions;
actions[0] = std::make_pair(Symbol('X'), Moves::RIGHT);
Transition t(q0, q1, Symbol('a'), actions);
```

---

#### 6. **FileParser** (`file_parser.h`, `file_parser.cc`)

**Responsabilidad:** Parsear ficheros de definición de MT.

**Métodos públicos:**
```cpp
TuringMachine parseFile(const std::string& filename);
```

**Proceso de parseo:**
1. Lee las 7 secciones del fichero
2. Construye objetos `State`, `Symbol`, `Alphabet`
3. Parsea transiciones línea por línea
4. Construye el map de acciones por cinta directamente
5. Retorna un objeto `TuringMachine`

**Ejemplo de código interno:**
```cpp
// Parseo de transiciones
std::map<int, std::pair<Symbol, Moves>> tapeActions;
int tapeIndex = 0;
while (ss >> writeSymStr >> moveStr) {
  Symbol writeSymbol = Symbol(writeSymStr[0]);
  Moves move = (moveStr[0] == 'L') ? Moves::LEFT : 
               (moveStr[0] == 'R') ? Moves::RIGHT : Moves::STAY;
  tapeActions.insert(std::make_pair(tapeIndex, std::make_pair(writeSymbol, move)));
  tapeIndex++;
}
Transition t(from, to, readSymbol, tapeActions);
```

---

#### 7. **TuringMachine** (`turing_machine.h`, `turing_machine.cc`)

**Responsabilidad:** Facade que coordina Model y Simulator.

**Atributos privados:**
```cpp
TuringMachineModel model_;      // Datos de la MT
TuringMachineSimulator simulator_; // Lógica de ejecución
```

**Métodos públicos:**
```cpp
TuringMachine(std::vector<State> states, std::vector<Transition> transitions, 
              Alphabet stringAlphabet, Alphabet tapeAlphabet, 
              const std::string& initialStateId);
bool compute(String& input, bool trace, std::ostream& os) const;
const TuringMachineModel& getModel() const;
```

**Patrón Facade:**
```cpp
// Constructor inicializa model_ y simulator_ con objetos directos (no punteros)
TuringMachine::TuringMachine(...)
  : model_(states, transitions, stringAlphabet, tapeAlphabet, initialStateId),
    simulator_(model_) {
}

// compute() delega al simulador
bool TuringMachine::compute(String& input, bool trace, std::ostream& os) const {
  return simulator_.compute(input, trace, os);
}
```

---

#### 8. **TuringMachineModel** (`turing_machine_model.h`, `turing_machine_model.cc`)

**Responsabilidad:** Almacenar la estructura de la MT (estados, transiciones, alfabetos).

**Atributos privados:**
```cpp
std::unordered_map<std::string, State> statesMap_;           // Estados por ID
std::map<std::string, std::vector<Transition>> transitionsMap_; // Transiciones por estado
Alphabet stringAlphabet_;                                    // Alfabeto de entrada
Alphabet tapeAlphabet_;                                      // Alfabeto de cinta
std::string initialStateId_;                                 // ID del estado inicial
```

**Optimizaciones:**
- ✅ `unordered_map` para búsqueda O(1) de estados
- ✅ `map` para transiciones ordenadas por estado origen
- ✅ Getters devuelven `const&` para evitar copias

**Métodos públicos:**
```cpp
const std::unordered_map<std::string, State>& getStates() const;
const std::map<std::string, std::vector<Transition>>& getTransitions() const;
const State& getInitialState() const; // Lanza excepción si no existe
const State& getStateById(const std::string& id) const;
```

**Ejemplo de uso:**
```cpp
const State& currentState = model.getStateById("q0");
const auto& transitions = model.getTransitions();
```

---

#### 9. **TuringMachineSimulator** (`turing_machine_simulator.h`, `turing_machine_simulator.cc`)

**Responsabilidad:** Ejecutar la simulación de la MT.

**Atributos privados:**
```cpp
const TuringMachineModel& model_; // Referencia al modelo (no copia)
TracePrinter tracePrinter_;       // Impresora de trazas (composición)
```

**Método principal:**
```cpp
bool compute(String& input, bool trace, std::ostream& os) const;
```

**Algoritmo de simulación:**

```cpp
bool TuringMachineSimulator::compute(String& input, bool trace, std::ostream& os) const {
  // 1. Inicializar cintas
  std::map<int, Tape> tapes;
  tapes[0] = Tape(input);
  
  // 2. Estado inicial
  const State* currentState = &model_.getInitialState();
  
  // 3. Bucle principal
  int steps = 0;
  const int MAX_STEPS = 10000;
  
  while (steps < MAX_STEPS) {
    // a) Imprimir traza si está activa
    if (trace) {
      tracePrinter_.printStep(os, steps, *currentState, tapes, ...);
    }
    
    // b) ¿Estado de aceptación?
    if (currentState->isAccept()) {
      if (trace) tracePrinter_.printAcceptedMessage(os);
      return true;
    }
    
    // c) Leer símbolo de cinta 0
    Symbol readSymbol = tapes[0].read();
    
    // d) Buscar transición aplicable
    const Transition* applicableTransition = nullptr;
    for (const auto& t : model_.getTransitions().at(currentState->getId())) {
      if (t.getReadSymbol() == readSymbol) {
        applicableTransition = &t;
        break;
      }
    }
    
    // e) Si no hay transición, rechazar
    if (!applicableTransition) {
      if (trace) tracePrinter_.printRejectedMessage(os);
      return false;
    }
    
    // f) Aplicar transición
    for (const auto& [tapeIndex, action] : applicableTransition->getTapeActions()) {
      const Symbol& writeSymbol = action.first;
      const Moves& move = action.second;
      
      tapes[tapeIndex].write(writeSymbol);
      tapes[tapeIndex].move(move);
    }
    
    // g) Cambiar de estado
    currentState = &model_.getStateById(applicableTransition->getTo().getId());
    steps++;
  }
  
  // Máximo de pasos alcanzado
  if (trace) tracePrinter_.printMaxStepsMessage(os);
  return false;
}
```

---

#### 10. **TracePrinter** (`trace_printer.h`, `trace_printer.cc`)

**Responsabilidad:** Formatear y imprimir trazas de ejecución (separación de presentación).

**Métodos públicos:**
```cpp
void printHeader(std::ostream& os) const;
void printStep(std::ostream& os, int stepNumber, const State& currentState,
               const std::map<int, Tape>& tapes, const Transition& transition) const;
void printAcceptedMessage(std::ostream& os) const;
void printRejectedMessage(std::ostream& os) const;
void printMaxStepsMessage(std::ostream& os) const;
```

**Salida formateada:**
```
╔═══════════════════════════════════════════════╗
║  PASO 0                                       ║
╠═══════════════════════════════════════════════╣
║  Estado actual: q0                            ║
║  Símbolos leídos: cinta0=[a], cinta1=[.]      ║
║  Transición: q0 → q1                          ║
║  Acciones:    cinta0: escribir 'X', mover →   ║
║               cinta1: escribir '.', mover →   ║
╠═══════════════════════════════════════════════╣
║  Estado de las cintas:    cinta0: [X]         ║
║                           cinta1: [.]         ║
╚═══════════════════════════════════════════════╝
```

**Patrón de composición:**
```cpp
class TuringMachineSimulator {
  TracePrinter tracePrinter_; // Composición (no herencia)
  
  void compute(...) {
    if (trace) {
      tracePrinter_.printStep(os, ...); // Delega la impresión
    }
  }
};
```

---

#### 11. **String** y **Tape**

**String** (`string.h`, `string.cc`): Representa una cadena como vector de símbolos.

**Tape**: Representa una cinta de la MT con:
- Vector de símbolos
- Posición del cabezal
- Operaciones: `read()`, `write()`, `move()`

---

## 🔧 Compilación y Ejecución

### Compilar

```bash
cd pract-2
make
```

El `Makefile` detecta automáticamente todos los ficheros `.cc` en `src/` y genera el ejecutable `pract-02`.

### Limpiar

```bash
make clean
```

### Ejecutar

**Sintaxis:**
```bash
./pract-02 <fichero_MT> <fichero_cadenas> [--trace]
```

**Parámetros:**
- `<fichero_MT>`: Ruta al fichero de definición de la MT
- `<fichero_cadenas>`: Ruta al fichero con cadenas de entrada
- `[--trace]`: (Opcional) Activa el modo traza detallado

**Salida:**
- Resultados se guardan en `FileOut.txt`
- Si `--trace` está activo, la traza se imprime en consola

---

## 📝 Ejemplos de Uso

### Ejemplo 1: MT simple sin traza

```bash
./pract-02 Inputs/MT/EvenAs_1Tape_MT.txt Inputs/Strigs/strings_evenas.txt
```

**Salida en `FileOut.txt`:**
```
aa: ACEPTADA -> Resultado: aa.
aaaa: ACEPTADA -> Resultado: aaaa.
a: RECHAZADA -> Resultado: a.
aaa: RECHAZADA -> Resultado: aaa.
```

### Ejemplo 2: MT con traza

```bash
./pract-02 Inputs/MT/BinaryReverse_2Tapes_MT.txt Inputs/Strigs/strings_binary.txt --trace
```

**Salida en consola:**
```
========================================================
PROCESANDO CADENA: "101"
========================================================

TRAZA DE EJECUCIÓN
========================================================

╔═══════════════════════════════════════════════════╗
║  PASO 0                                           ║
╠═══════════════════════════════════════════════════╣
║  Estado actual: q0                                ║
║  Símbolos leídos: cinta0=[1], cinta1=[.]          ║
║  Transición: q0 → q0                              ║
║  Acciones:    cinta0: escribir '1', mover →(der)  ║
║               cinta1: escribir '1', mover →(der)  ║
╠═══════════════════════════════════════════════════╣
║  Estado de las cintas:    cinta0: [1] 0 1         ║
║                           cinta1: [.] . .         ║
╚═══════════════════════════════════════════════════╝

[... más pasos ...]

RESULTADO FINAL: 101 -> ✓ ACEPTADA
Cinta resultante: ...
```

### Ejemplo 3: Script de prueba automática

```bash
./test_all.sh
```

Ejecuta todas las MTs de ejemplo y muestra los resultados.

---

## 🎨 Características de Diseño

### 1. **Sin Punteros, Solo Referencias**

Todo el código usa referencias (`&`) en lugar de punteros (`*`):

```cpp
// ✅ Bueno: Referencias
const State& getStateById(const std::string& id) const {
  auto it = statesMap_.find(id);
  if (it == statesMap_.end()) {
    throw std::runtime_error("Estado no encontrado: " + id);
  }
  return it->second;
}

// ❌ Malo: Punteros (versión antigua)
const State* getStateById(const std::string& id) const {
  auto it = statesMap_.find(id);
  if (it == statesMap_.end()) {
    return nullptr; // Requiere verificación en cada uso
  }
  return &it->second;
}
```

### 2. **Composición sobre Herencia**

El `TuringMachineSimulator` **contiene** un `TracePrinter` (composición) en lugar de heredar de él:

```cpp
class TuringMachineSimulator {
  TracePrinter tracePrinter_; // Composición: "tiene-un" TracePrinter
};
```

### 3. **Objetos Directos en lugar de unique_ptr**

```cpp
class TuringMachine {
  TuringMachineModel model_;      // ✅ Objeto directo
  TuringMachineSimulator simulator_; // ✅ Objeto directo
  
  // ❌ Versión antigua:
  // std::unique_ptr<TuringMachineModel> model_;
  // std::unique_ptr<TuringMachineSimulator> simulator_;
};
```

### 4. **Encapsulación con Getters**

```cpp
class Args {
private:
  std::string mtFile_;
  std::string stringsFile_;
  bool trace_;
  
public:
  const std::string& getMtFile() const { return mtFile_; }
  const std::string& getStringsFile() const { return stringsFile_; }
  bool getTrace() const { return trace_; }
};
```

### 5. **Separación de Responsabilidades**

- **Model**: Solo datos
- **Simulator**: Solo lógica de ejecución
- **TracePrinter**: Solo presentación
- **FileParser**: Solo parseo

---

## 📄 Licencia

Este proyecto es material académico para la asignatura de Complejidad Computacional.

---

**Última actualización:** 20 Octubre 2025
