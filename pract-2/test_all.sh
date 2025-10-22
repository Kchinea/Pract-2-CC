#!/bin/bash

# Script para probar todas las máquinas de Turing de ejemplo
# Uso: ./test_all.sh

echo "=========================================="
echo "PROBANDO TODAS LAS MÁQUINAS DE TURING"
echo "=========================================="
echo ""

# 1. Par de a's (1 cinta)
echo "1. Número par de 'a's (1 cinta)"
echo "-----------------------------------"
./pract-02 Inputs/MT/EvenAs_1Tape_MT.txt Inputs/Strigs/strings_evenas.txt
cat FileOut.txt
echo ""

# 2. a*b* (1 cinta)
echo "2. Lenguaje a*b* (1 cinta)"
echo "-----------------------------------"
./pract-02 Inputs/MT/AStarBStar_1Tape_MT.txt Inputs/Strigs/strings_astarb.txt
cat FileOut.txt
echo ""

# 3. a^n b^m con m>n (1 cinta)
echo "3. a^n b^m donde m > n (1 cinta)"
echo "-----------------------------------"
./pract-02 Inputs/MT/AnBn_Plus_MT.txt Inputs/Strigs/strings_anbn_plus.txt
cat FileOut.txt
echo ""

# 4. Reversión binaria (2 cintas)
echo "4. Reversión binaria (2 cintas)"
echo "-----------------------------------"
./pract-02 Inputs/MT/BinaryReverse_2Tapes_MT.txt Inputs/Strigs/strings_binary.txt
cat FileOut.txt
echo ""

# 5. Copia (2 cintas)
echo "5. Copia a segunda cinta (2 cintas)"
echo "-----------------------------------"
./pract-02 Inputs/MT/Copy_2Tapes_MT.txt Inputs/Strigs/strings_copy.txt
cat FileOut.txt
echo ""

echo "=========================================="
echo "PRUEBAS COMPLETADAS"
echo "=========================================="
