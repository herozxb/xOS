nasm -f bin Stage3.asm -o KRNL.SYS
ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary



# step 2 to copy to the floppy disk

sudo mount -o loop floppy.img /media/floppy/
sudo cp KRNLDR.SYS /media/floppy
sudo cp KRNL.SYS /media/floppy
sudo umount /media/floppy
qemu-system-i386 -fda floppy.img



# test 3 for kernel
gcc -ffreestanding -m32 -c kernel.c -o kernel.o
nasm kernel_entry.asm -f elf -o kernel_entry.o
ld -o KRNL.SYS  -m elf_i386 -Ttext 0x100000 kernel_entry.o kernel.o --oformat binary

# step 4 add super level function
gcc -ffreestanding -m32 -c DebugDisplay.c -o DebugDisplay.o
gcc -ffreestanding -m32 -c string.c -o string.o
ld -o KRNL.SYS  -m elf_i386 -Ttext 0x100000 kernel_entry.o kernel.o DebugDisplay.o string.o --oformat binary

# step 5
gcc -ffreestanding -m32 -c Hal.c -o Hal.o
gcc -ffreestanding -m32 -c cpu.c -o cpu.o
gcc -ffreestanding -m32 -c gdt.c -o gdt.o
gcc -ffreestanding -m32 -c idt.c -o idt.o
ld -o KRNL.SYS  -m elf_i386 -Ttext 0x100000 kernel_entry.o kernel.o DebugDisplay.o string.o Hal.o cpu.o gdt.o  --oformat binary


# step 6 debug
objdump -D -b binary -m i386:x86-64 KRNL.SYS
target remote :1234