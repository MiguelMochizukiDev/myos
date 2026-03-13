/* kernel.c */

#include "../include/vga.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/pic.h"

void kernel_main(void) {
	terminal_init();

	terminal_setcolor(VGA_LIGHT_GREEN, VGA_BLACK);
	kprint("myOS kernel\n");
	terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);

	gdt_init();
	kprint("GDT loaded!\n");

	pic_remap();
	kprint("PIC remapped!\n");

	idt_init();
	kprint("IDT loaded!\n");

	__asm__ volatile ("sti");
	kprint("Interrupts enabled!\n");

	volatile int zero = 0;
	volatile int x = 1 / zero;
	(void) x;

	while (1) {};
}
