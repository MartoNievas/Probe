CC=cc
CFLAGS= -std=c11 -Wall -Wextra -pedantic -O0 -g -lm -Wno-unused-variable -Wno-unused-parameter -no-pie -z noexecstack -I ../
NASM=nasm
NASMFLAGS=-f elf64 -g -F DWARF
NASM_ABI_FLAGS=$(NASMFLAGS) -p "abi_enforcer.mac"
STRUCTS_FILE=structs.h
STRUCTS_PATH="../6\ -\ Debugging"

all: main tester abi_enforcer_tester abi_enforcer_main 

C_OBJS := Debugging_c.o
ASM_OBJS := Debugging_asm.o Debugging_c.o
ABI_OBJS := Debugging_abi.o abi_enforcer_regsave.o Debugging_c.o

tester: tester.o $(ASM_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

main: main.o $(ASM_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

abi_enforcer_tester: tester.o $(ABI_OBJS)
	$(CC) $(CFLAGS) -DWITH_ABI_ENFORCER=1 $^ -o $@

abi_enforcer_main: main.o $(ABI_OBJS)
	$(CC) $(CFLAGS) -DWITH_ABI_ENFORCER=1 $^ -o $@

tester.o: tester.c
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

Debugging_c.o: Debugging.c
	$(CC) $(CFLAGS) -c $< -o $@

Debugging_asm.o: Debugging.asm
	$(NASM) $(NASMFLAGS) $< -o $@

Debugging_abi.o: Debugging.asm abi_enforcer.mac
	@echo Revisando offsets y tamaños de structs declarados en checkpoint...
	@cd ../test_utils && $(MAKE) INPUT_C=$(STRUCTS_FILE) INPUT_ASM=$< INPUT_PATH=$(STRUCTS_PATH) --no-print-directory
	@echo
	$(NASM) $(NASM_ABI_FLAGS) $< -o $@

abi_enforcer_regsave.o: abi_enforcer_regsave.asm
	$(NASM) $(NASMFLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f tester tester_c main main_c abi_enforcer_tester abi_enforcer_main
	rm -f salida.propios.*
	@cd ../test_utils && $(MAKE) clean --no-print-directory
	
.PHONY: Debugging_abi.o