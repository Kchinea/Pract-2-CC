# Práctica 2 - Simulador de Máquina de Turing

Este proyecto implementa un simulador de Máquina de Turing (MT) en C++. Permite leer la definición de una MT y un fichero de cadenas, procesando cada cadena y mostrando si es aceptada o rechazada.

## Formato de los archivos de entrada

### Archivo de definición de la MT (ver ejemplos en `Inputs/`)
El archivo debe tener el siguiente formato (una línea por sección):

1. Lista de estados (separados por espacio)
2. Alfabeto de entrada (símbolos separados por espacio)
3. Alfabeto de la cinta (símbolos separados por espacio, incluir el símbolo blanco: `.`)
4. Estado inicial
5. Símbolo blanco de la cinta (normalmente `.`)
6. Estado de aceptación
7. Transiciones (una por línea, formato: `estado_lectura símbolo_lectura símbolo_escritura estado_siguiente dirección`)
	- Ejemplo: `q0 1 a q1 R` (desde q0, si lee 1, escribe a, va a q1 y mueve la cabeza a la derecha)

### Archivo de cadenas de entrada
Un archivo de texto donde cada línea contiene una cadena a evaluar por la MT.

## Ejecución

Compila el proyecto con el `Makefile` incluido:

```sh
make
```

Ejecuta el simulador pasando como argumentos:

```
./pract-02 <fichero_MT> <fichero_cadenas> [--trace]
```

- `<fichero_MT>`: Ruta al archivo de definición de la máquina de Turing.
- `<fichero_cadenas>`: Ruta al archivo con las cadenas a evaluar (una por línea).
- `--trace` (opcional): Si se incluye, se mostrará el trazo de la ejecución de la MT para cada cadena.

El resultado se guarda en `FileOut.txt`.

## Salida

Por cada cadena, se indica si es **ACEPTADA** o **RECHAZADA**. Si se activa el modo traza, se muestra el detalle de la ejecución paso a paso.

## Ejemplo de uso

```
./pract-02 Inputs/Ejemplo2_MT.txt Inputs/cadenas.txt --trace
```

## Notas
- El símbolo blanco de la cinta debe ser `.`
- El estado de aceptación es el último de la lista de estados.
- El simulador detiene la ejecución si no encuentra transición aplicable o si alcanza el estado de aceptación.