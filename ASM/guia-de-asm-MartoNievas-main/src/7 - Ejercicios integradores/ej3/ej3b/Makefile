AS = nasm
ASFLAGS = -felf64 -Fdwarf
LDFLAGS = -lm
CFLAGS= -Wall -Wextra -Wno-pedantic -O0 -g -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter -no-pie -z noexecstack -I ../../../test_utils/
VPATH=.:../../../test_utils
STRUCTS_FILE=../ejs.h
STRUCTS_PATH="../7\ -\ Ejercicios\ integradores/ej3/ej3b"
# Crea los binarios en cuestión
all: test_c test_asm #test_abi

# Corre los tests usando la implementación en C
run_c: test_c
	./test_c

# Corre los tests usando la implementación en ASM
run_asm: test_asm
	./test_asm

# Corre main usando la implementación en C
run_main_c: main_c
	./main_c

# Corre main usando la implementación en ASM
run_main_asm: main_asm
	./main_asm

run_abi: test_abi
	./test_abi

# Corre los tests en valgrind usando la implementación en C
valgrind_c: test_c
	valgrind  --show-reachable=yes --leak-check=full --error-exitcode=1 ./test_c \
	&& echo "No se detectaron errores de memoria"

# Corre los tests en valgrind usando la implementación en ASM
valgrind_asm: test_asm
	valgrind  --show-reachable=yes --leak-check=full --error-exitcode=1 ./test_asm \
	&& echo "No se detectaron errores de memoria"

valgrind_abi: test_abi
	valgrind  --show-reachable=yes --leak-check=full --error-exitcode=1 ./test_abi \
	&& echo "No se detectaron errores de memoria ni ABI (no incluye chequeo de offsets ni de preservación de no volátiles, que se loggean aparte)"

# Borra todos los archivos generados
clean:
	rm -f main_asm main_c test_c test_asm test_abi *.o
	@cd ../../../test_utils && $(MAKE) clean

test_c: test.c ej3b.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_asm: test.c ej3b_asm.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_abi: test.c ej3b_abi.o  abi_enforcer_regsave.o
	$(CC) $(CFLAGS) -DWITH_ABI_ENFORCER=1 $^ -o $@ $(LDFLAGS)

main_c: main.c ej3b.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

main_asm: main.c ej3b_asm.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

ej3b_asm.o: ej3b.asm
	$(AS) $(ASFLAGS) $^ -o $@

ej3b_abi.o: ej3b.asm
	@cd ../../../test_utils && $(MAKE) INPUT_C=$(STRUCTS_FILE) INPUT_ASM=$< INPUT_PATH=$(STRUCTS_PATH) --no-print-directory
	$(AS) $(ASFLAGS) -p "../../../test_utils/abi_enforcer.mac" $< -o $@

abi_enforcer_regsave.o: abi_enforcer_regsave.asm
	$(AS) $(ASFLAGS) $< -o $@

.PHONY: clean run_c run_asm run_abi main_c main_asm valgrind_c valgrind_asm valgrind_abi

