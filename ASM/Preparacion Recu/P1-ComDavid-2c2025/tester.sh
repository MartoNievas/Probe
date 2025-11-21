#!/bin/bash

(timeout 30s bash -c 'cd src/ej1 && make clean && make valgrind_abi && echo -e "\033[0;32mTests EJ1 corrieron exitosamente\033[0m"')
EJ1_STATUS=$?

(timeout 30s bash -c 'cd src/ej2 && make clean && make valgrind_abi && echo -e "\033[0;32mTests EJ2 corrieron exitosamente\033[0m"')
EJ2_STATUS=$?

(timeout 30s bash -c 'cd src/ej3 && make clean && make valgrind_abi && echo -e "\033[0;32mTests EJ3 corrieron exitosamente\033[0m"')
EJ3_STATUS=$?

ejercicios_correctos=0

if [ "$EJ1_STATUS" -eq 0 ]; then
    ((ejercicios_correctos++))
fi

if [ "$EJ2_STATUS" -eq 0 ]; then
    ((ejercicios_correctos++))
fi

if [ "$EJ3_STATUS" -eq 0 ]; then
    ((ejercicios_correctos++))
fi

if [ "$ejercicios_correctos" -le 1 ]; then
    echo
    echo -e '\033[0;31mDesaprobado: Hay errores en más de un ejercicio\033[0m'
    exit 1
else
    echo
    echo -e '\033[0;32mAprobado: Al menos dos ejercicios no tienen errores\033[0m'
    exit 0
fi
