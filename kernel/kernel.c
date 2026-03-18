/* kernel.c */

#include "drivers/vga.h"
#include "cpu/gdt.h"
#include "cpu/pic.h"
#include "cpu/idt.h"
#include "drivers/timer.h"

void kernel_main(void) {
	terminal_init();

	terminal_setcolor(VGA_LIGHT_GREEN, VGA_BLACK);
	kprint("myOS kernel\n");
	terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);

	gdt_init();
	kprint("GDT loaded!\n");

	pic_remap();
	pic_mask_all();
	kprint("PIC remapped!\n");

	idt_init();
	kprint("IDT loaded!\n");

	timer_init();
	kprint("Timer started\n");

	__asm__ volatile ("sti");
	kprint("Interrupts enabled!\n");

	while (1) {};
}
