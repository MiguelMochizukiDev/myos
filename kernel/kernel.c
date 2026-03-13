/* kernel.c */

#include "../include/vga.h"
#include "../include/gdt.h"

void kernel_main(void) {
	terminal_init();

	terminal_setcolor(VGA_LIGHT_GREEN, VGA_BLACK);
	kprint("myOS kernel\n");

	terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);

	gdt_init();
	kprint("GDT Loaded!\n");

	kprint("VGA buffer in: ");
	kprint_hex(0xB8000);
	kputchar('\n');

	kprint("Fuck Microsoft!\n");

	kprint("Love you Linus GOAT <3\n");

	while (1) {};
}
