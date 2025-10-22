#!/bin/bash

# Script para probar todas las máquinas de Turing
# Uso: ./test_all.sh

# Colores para output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Crear directorio de outputs si no existe
mkdir -p Test/Outputs

echo "=========================================="
echo "PROBANDO TODAS LAS MÁQUINAS DE TURING"
echo "=========================================="
echo ""

# Función para ejecutar test
run_test() {
    local mt_file=$1
    local strings_file=$2
    local output_file=$3
    local description=$4
    
    echo -e "${BLUE}${description}${NC}"
    echo "-----------------------------------"
    ./pract-02 "$mt_file" "$strings_file" > "$output_file" 2>&1
    cat FileOut.txt
    mv FileOut.txt "$output_file.results"
    echo ""
}

# ============================================
# EJERCICIOS DEL README (Inputs/)
# ============================================

echo -e "${GREEN}=== EJERCICIOS DEL README ===${NC}"
echo ""

# Ejercicio 1: a^n b^m donde m > n, n >= 1
run_test "Inputs/MT/AnBn_Plus_MT.txt" \
         "Inputs/Strigs/strings_anbn_plus.txt" \
         "Test/Outputs/anbn_plus.out" \
         "Ejercicio 1: a^n b^m donde m > n, n ≥ 1"
echo -e "${YELLOW}Debería ACEPTAR: abb, abbb, aabbb, aaabbbbb${NC}"
echo -e "${YELLOW}Debería RECHAZAR: ab, aab, aaabbb (m no es > n)${NC}"
echo ""

# Ejercicio 2: Contador de a's y b's (3 cintas)
run_test "Inputs/MT/CountAB_2Tapes_MT.txt" \
         "Inputs/Strigs/strings_ab.txt" \
         "Test/Outputs/countab.out" \
         "Ejercicio 2: Contador de a's y b's en formato unario (3 cintas)"
echo -e "${YELLOW}Convierte a .1^(n_a+1).1^(n_b+1).${NC}"
echo -e "${YELLOW}Ejemplos: a → .11.1. | aa → .111.1. | abb → .11.111.${NC}"
echo ""

# ============================================
# MÁQUINAS DE TEST (Test/MT/)
# ============================================

echo -e "${GREEN}=== MÁQUINAS DE TEST ===${NC}"
echo ""

# 1. Número par de 'a's (1 cinta)
run_test "Test/MT/EvenAs_1Tape_MT.txt" \
         "Test/Strings/strings_evenas.txt" \
         "Test/Outputs/evenas.out" \
         "1. Número par de 'a's (1 cinta)"
echo -e "${YELLOW}Debería ACEPTAR: ε (vacía), aa, aaaa, bb, aabb, abab${NC}"
echo -e "${YELLOW}Debería RECHAZAR: a, aaa, aba${NC}"
echo ""

# 2. a*b* (1 cinta)
run_test "Test/MT/AStarBStar_1Tape_MT.txt" \
         "Test/Strings/strings_astarb.txt" \
         "Test/Outputs/astarb.out" \
         "2. Lenguaje a*b* (1 cinta)"
echo -e "${YELLOW}Debería ACEPTAR: ε, a, aa, b, bb, ab, aab, abb, aabb${NC}"
echo -e "${YELLOW}Debería RECHAZAR: ba, aba, bab${NC}"
echo ""

# 3. Reversión binaria (2 cintas)
run_test "Test/MT/BinaryReverse_2Tapes_MT.txt" \
         "Test/Strings/strings_binary.txt" \
         "Test/Outputs/binary_reverse.out" \
         "3. Reversión binaria (2 cintas)"
echo -e "${YELLOW}Invierte cadenas binarias: 10→01, 110→011, 1010→0101${NC}"
echo ""

# 4. Copia (2 cintas)
run_test "Test/MT/Copy_2Tapes_MT.txt" \
         "Test/Strings/strings_copy.txt" \
         "Test/Outputs/copy.out" \
         "4. Copia a segunda cinta (2 cintas)"
echo -e "${YELLOW}Copia cinta 0 a cinta 1. Todas deberían ser ACEPTADAS${NC}"
echo ""

# 5. Palíndromos (3 cintas)
run_test "Test/MT/Palindrome_3Tapes_MT.txt" \
         "Test/Strings/strings_palindrome.txt" \
         "Test/Outputs/palindrome.out" \
         "5. Palíndromos (3 cintas)"
echo -e "${YELLOW}Debería ACEPTAR: a, aa, aba, aaa, abba, aabaa${NC}"
echo -e "${YELLOW}Debería RECHAZAR: ab, abc, aab, abb${NC}"
echo ""

# 6. Ejemplo 1: Impar de 0's
run_test "Test/MT/Ejemplo_MT.txt" \
         "Test/Strings/strings_binary.txt" \
         "Test/Outputs/ejemplo1.out" \
         "6. Ejemplo 1: Número impar de 0's"
echo -e "${YELLOW}Debería ACEPTAR: 0, 000, 00000${NC}"
echo -e "${YELLOW}Debería RECHAZAR: ε, 00, 0000${NC}"
echo ""

# 7. Ejemplo 2
run_test "Test/MT/Ejemplo2_MT.txt" \
         "Test/Strings/test_simple.txt" \
         "Test/Outputs/ejemplo2.out" \
         "7. Ejemplo 2"
echo -e "${YELLOW}Máquina de prueba${NC}"
echo ""

echo "=========================================="
echo "PRUEBAS COMPLETADAS"
echo "=========================================="
echo -e "${GREEN}Resultados guardados en Test/Outputs/${NC}"
echo ""
echo "Para ver resultados detallados:"
echo "  cat Test/Outputs/anbn_plus.out.results"
echo "  cat Test/Outputs/countab.out.results"
echo "  cat Test/Outputs/evenas.out.results"
echo ""
echo "Archivos generados:"
ls -lh Test/Outputs/ 2>/dev/null | tail -n +2 | awk '{print "  " $9}'
echo ""
