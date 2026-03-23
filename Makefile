CC      := x86_64-elf-gcc
AS      := nasm
LD      := x86_64-elf-ld
GRUB    := grub-mkrescue

CFLAGS  := -std=c11 \
            -ffreestanding \
            -fno-stack-protector \
            -fno-pic \
            -mno-red-zone \
            -mno-mmx -mno-sse -mno-sse2 \
            -Wall -Wextra -Werror \
            -O2 \
            -I include

ASFLAGS := -f elf64
LDFLAGS := -nostdlib -T linker.ld -z max-page-size=0x1000

KERNEL  := iso/boot/myos.kernel
ISO     := myos.iso

SRCS_ASM := loader/boot.asm
SRCS_C   := kernel/main.c \
            kernel/kprintf.c \
            drivers/vga.c

OBJS := $(SRCS_ASM:.asm=.o) \
        $(SRCS_C:.c=.o)

.PHONY: all clean run debug

all: $(ISO)

$(ISO): $(KERNEL)
	$(GRUB) -o $@ iso

$(KERNEL): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

run: $(ISO)
	qemu-system-x86_64 -cdrom $(ISO) -m 128M -serial stdio

debug: $(ISO)
	qemu-system-x86_64 -cdrom $(ISO) -m 128M -serial stdio \
	    -d int,cpu_reset -no-reboot

clean:
	rm -f $(OBJS) $(KERNEL) $(ISO)
