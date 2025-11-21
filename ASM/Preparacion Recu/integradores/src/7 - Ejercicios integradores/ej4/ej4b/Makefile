AS = nasm
ASFLAGS = -felf64 -Fdwarf -g
CFLAGS= -Wall -Wextra -O0 -g -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter -no-pie -z noexecstack -I ../../../test_utils/
VPATH=.:../../../test_utils
LDFLAGS = -no-pie -z noexecstack
VALGRIND = valgrind  --show-reachable=yes --leak-check=full --error-exitcode=1

default: test_asm

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
	$(VALGRIND) ./test_c \
	&& echo "No se detectaron errores de memoria"

valgrind_asm: test_asm
	$(VALGRIND) ./test_asm \
	&& echo "No se detectaron errores de memoria"

valgrind_abi: test_abi
	$(VALGRIND) ./test_abi \
	&& echo "No se detectaron errores de memoria ni ABI"

test_c: ej4b_c.o test_ej4b.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_asm: ej4b_asm.o test_ej4b.o
	
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_abi: test_ej4b.c ej4b_abi.o abi_enforcer_regsave.o
	$(CC) $(CFLAGS) -DWITH_ABI_ENFORCER=1 $^ -o $@ $(LDFLAGS)

main_c: main.c ej4b.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

main_asm: main.c ej4b_asm.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

ej4b_c.o: ej4b.c
	$(CC) $(CFLAGS) -c $^ -o $@

ej4b_asm.o: ej4b.asm
	$(AS) $(ASFLAGS) $^ -o $@

ej4b_abi.o: ej4b.asm
	$(AS) $(ASFLAGS) -p "../../../test_utils/abi_enforcer.mac" $< -o $@

abi_enforcer_regsave.o: abi_enforcer_regsave.asm
	$(AS) $(ASFLAGS) $< -o $@
clean:
	rm -f test_c test_asm test_abi main_asm main_c *.o
