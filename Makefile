CC      = i686-elf-gcc
AS      = nasm

CFLAGS  = -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib

all: myos.bin

boot/boot.o: boot/boot.asm
	$(AS) -f elf32 boot/boot.asm -o boot/boot.o

cpu/gdt.o: cpu/gdt.c
	$(CC) $(CFLAGS) -c cpu/gdt.c -o cpu/gdt.o

cpu/gdt_flush.o: cpu/gdt_flush.asm
	$(AS) -f elf32 cpu/gdt_flush.asm -o cpu/gdt_flush.o

kernel/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel/kernel.o

drivers/vga.o: drivers/vga.c
	$(CC) $(CFLAGS) -c drivers/vga.c -o drivers/vga.o

myos.bin: boot/boot.o cpu/gdt.o cpu/gdt_flush.o kernel/kernel.o drivers/vga.o
	$(CC) $(LDFLAGS) -o myos.bin boot/boot.o cpu/gdt.o cpu/gdt_flush.o \
	    kernel/kernel.o drivers/vga.o -lgcc

clean:
	rm -f boot/boot.o cpu/gdt.o cpu/gdt_flush.o \
	    kernel/kernel.o drivers/vga.o myos.bin

iso: myos.bin
	cp myos.bin isodir/boot/myos.bin
	grub-mkrescue -o myos.iso isodir

run: iso
	qemu-system-i386 -cdrom myos.iso -boot order=d
