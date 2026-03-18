CC      = i686-elf-gcc
AS      = nasm
CFLAGS  = -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib

C_OBJS   := $(patsubst %.c,%.o,$(wildcard cpu/*.c drivers/*.c kernel/*.c))
ASM_OBJS := boot/boot.o cpu/gdt_flush.o cpu/idt_load.o cpu/isr_asm.o

OBJS := $(C_OBJS) $(ASM_OBJS)

all: myos.bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot/boot.o: boot/boot.asm
	$(AS) -f elf32 $< -o $@

cpu/gdt_flush.o: cpu/gdt_flush.asm
	$(AS) -f elf32 $< -o $@

cpu/idt_load.o: cpu/idt_load.asm
	$(AS) -f elf32 $< -o $@

cpu/isr_asm.o: cpu/isr.asm
	$(AS) -f elf32 $< -o $@

myos.bin: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) -lgcc

clean:
	rm -f $(C_OBJS) $(ASM_OBJS) myos.bin myos.iso
	rm -f isodir/boot/myos.bin

iso: myos.bin
	cp myos.bin isodir/boot/myos.bin
	grub-mkrescue -o myos.iso isodir

run: iso
	qemu-system-i386 -cdrom myos.iso -boot order=d
