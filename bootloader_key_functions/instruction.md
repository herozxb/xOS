## important
nasm boot.asm -f bin -o boot.bin

nasm loader.asm -f elf32 -o loader_entry.o
gcc -O0 -g -ffreestanding -m32 -c loader.c -o loader.o
ld -o loader.bin -m elf_i386 -Ttext 0x1000 loader_entry.o loader.o --oformat binary

cat boot.bin loader.bin > image.bin
qemu-system-i386 -fda image.bin
