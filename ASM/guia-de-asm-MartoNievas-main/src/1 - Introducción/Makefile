AS := nasm
ASFLAGS := -f elf64 -F DWARF -g -Wall
LD := ld
LDFLAGS := -g
TARGET := HolaMundo

.PHONY: all clean

all: $(TARGET)

# assembly
HolaMundo.o: HolaMundo.asm
	$(AS) $(ASFLAGS) $<

# linking
$(TARGET): HolaMundo.o
	$(LD) $(LDFLAGS) $< -o $@

clean:
	rm -rf *.o $(TARGET)
