PYTHON = python3

diff: asm_defs.txt $(INPUT_PATH)/$(INPUT_C) venv
	@venv/bin/python3 parse.py $(INPUT_PATH)/$(INPUT_C) | gcc -x c - -o test
	@./test | venv/bin/python3 diff.py
	@rm test

asm_defs.txt: $(INPUT_PATH)/$(INPUT_ASM) venv
	@nasm -f dbg $(INPUT_PATH)/$(INPUT_ASM) --before "BITS 64" -o checker.o 
	cat checker.o | venv/bin/python3 parse_nasm_dbg.py > asm_defs.txt ; rm checker.o

venv:
	conda deactivate || true # desactivar conda si es que está instalado y activado
	$(PYTHON) -m venv venv
	venv/bin/pip install --no-index --find-links=python_packages/ pycparser
	touch venv

clean:
	rm -f asm_defs.txt global_foos.txt

.PHONY: asm_defs.txt
