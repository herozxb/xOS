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
tss.o: tss.cpp 
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
pic.o: pic.cpp 
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
pit.o: pit.cpp 
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
mmngr_phys.o: mmngr_phys.cpp 
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
mmngr_virtual.o: mmngr_virtual.cpp 
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
vmmngr_pde.o: vmmngr_pde.cpp 
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
vmmngr_pte.o: vmmngr_pte.cpp
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
kybrd.o: kybrd.cpp
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
stdio.o: stdio.cpp
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
keyboard.o: keyboard.cpp
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
terminal.o: terminal.cpp
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@
flpydsk.o: flpydsk.cpp
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@	
fat12.o: fat12.cpp
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@	
ide.o: ide.cpp
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@	
#sysfile.o: sysfile.cpp
#	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@	
fsys.o: fsys.cpp
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@	
ctype.o: ctype.cpp
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@	
fat12a.o: fat12a.cpp
	gcc -O0 -ffreestanding -m32 -ggdb -c $< -o $@	
	
kernel_entry.o: kernel_entry.asm
	nasm $< -f elf -o $@

#geninterrupt.o: geninterrupt.asm
#	nasm $< -f elf -o $@

KRNL.SYS: kernel.o \
          kernel_entry.o \
          DebugDisplay.o \
          string.o \
          Hal.o \
          cpu.o \
          gdt.o \
          idt.o \
          tss.o \
          pic.o \
          pit.o \
          mmngr_phys.o \
          mmngr_virtual.o \
          vmmngr_pde.o \
          vmmngr_pte.o \
          kybrd.o \
          stdio.o \
          keyboard.o \
          terminal.o \
          flpydsk.o \
          fat12a.o \
          ide.o \
          fsys.o \
          ctype.o \
          fat12.o 
       

	ld -o $@ -m elf_i386 -Ttext 0x100000 $^ --oformat binary
	

kernel.elf: kernel.o \
          kernel_entry.o \
          DebugDisplay.o \
          string.o \
          Hal.o \
          cpu.o \
          gdt.o \
          idt.o \
          tss.o \
          pic.o \
          pit.o \
          mmngr_phys.o \
          mmngr_virtual.o \
          vmmngr_pde.o \
          vmmngr_pte.o \
          kybrd.o \
          stdio.o \
          keyboard.o \
          terminal.o \
          flpydsk.o \
          fat12a.o \
          ide.o \
          fsys.o \
          ctype.o \
          fat12.o 

	ld -o $@ -m elf_i386 -Ttext 0x100000 $^ 

run: floppy.img
	qemu-system-i386 -fda $<
	
floppy:
	sudo mount -o loop floppy.img /media/floppy/
	sudo cp KRNL.SYS /media/floppy
	sudo umount /media/floppy

	
debug: floppy.img kernel.elf
	qemu-system-i386 -s -fda floppy.img &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf" -ex "b flpydsk_set_working_drive"
	
.PHONY: clean
clean:
	rm  *.o 
