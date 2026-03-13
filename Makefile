CC      = i686-elf-gcc
AS      = nasm

CFLAGS  = -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib

OBJS = boot/boot.o		\
       cpu/gdt.o		\
       cpu/gdt_flush.o	\
       cpu/idt.o		\
       cpu/idt_load.o	\
       cpu/isr.o		\
       cpu/isr_asm.o	\
	   cpu/pic.o		\
       kernel/kernel.o	\
       drivers/vga.o	\

all: myos.bin

boot/boot.o:       boot/boot.asm
	$(AS) -f elf32 $< -o $@

cpu/gdt_flush.o:   cpu/gdt_flush.asm
	$(AS) -f elf32 $< -o $@

cpu/idt_load.o:    cpu/idt_load.asm
	$(AS) -f elf32 $< -o $@

cpu/isr_asm.o:     cpu/isr.asm
	$(AS) -f elf32 $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

myos.bin: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) -lgcc

clean:
	rm -f $(OBJS) myos.bin

iso: myos.bin
	cp myos.bin isodir/boot/myos.bin
	grub-mkrescue -o myos.iso isodir

run: iso
	qemu-system-i386 -cdrom myos.iso -boot order=d \
		-no-reboot -no-shutdown -d int,cpu_reset 2>qemu.log
