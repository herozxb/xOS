# $@ = target file
# $< = first dependency
# $^ = all dependencies
all : KRNL.SYS

kernel.o: kernel.cpp
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
DebugDisplay.o: DebugDisplay.cpp 
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
string.o: string.cpp 
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
Hal.o: Hal.cpp 
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
cpu.o: cpu.cpp 
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
gdt.o: gdt.cpp 
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
idt.o: idt.cpp 
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
	
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
	

kernel.elf: kernel.o \
          kernel_entry.o \
          DebugDisplay.o \
          string.o \
          Hal.o \
          cpu.o \
          gdt.o \
          idt.o
	ld -o $@ -m elf_i386 -Ttext 0x100000 $^ 

run: floppy.img
	qemu-system-i386 -fda $<
	
debug: floppy.img kernel.elf
	qemu-system-i386 -s -fda floppy.img &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"	
	
.PHONY: clean
clean:
	rm  *.o 
