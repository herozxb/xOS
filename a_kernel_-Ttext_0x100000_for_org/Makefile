# $@ = target file
# $< = first dependency
# $^ = all dependencies
all : KRNL.SYS

kernel.o: kernel.c 
	gcc -ffreestanding -m32 -c $< -o $@
DebugDisplay.o: DebugDisplay.c 
	gcc -ffreestanding -m32 -c $< -o $@
string.o: string.c 
	gcc -ffreestanding -m32 -c $< -o $@
Hal.o: Hal.c 
	gcc -ffreestanding -m32 -c $< -o $@
cpu.o: cpu.c 
	gcc -ffreestanding -m32 -c $< -o $@
gdt.o: gdt.c 
	gcc -ffreestanding -m32 -c $< -o $@
idt.o: idt.c 
	gcc -ffreestanding -m32 -c $< -o $@
	
kernel_entry.o: kernel_entry.asm
	nasm $< -f elf -o $@

KRNL.SYS: kernel.o \
          kernel_entry.o \
          DebugDisplay.o \
          string.o \
          Hal.o \
          cpu.o \
          gdt.o \
          idt.o
	ld -o $@ -m elf_i386 -Ttext 0x100000 $^ --oformat binary

run: floppy.img
	qemu-system-i386 -fda $<
	
.PHONY: clean
clean:
	rm  *.o 
