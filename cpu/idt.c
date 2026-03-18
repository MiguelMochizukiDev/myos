/* idt.c */

#include "cpu/idt.h"
#include "cpu/isr.h"

#define IDT_ENTRIES 256

static idt_entry_t idt[IDT_ENTRIES];
static idt_ptr_t idt_ptr;

extern void idt_load(uint32_t);

static void idt_set_entry(int i, uint32_t handler, uint8_t flags) {
	idt[i].offset_low 	= handler & 0xFFFF;
	idt[i].offset_high 	= (handler >> 16) & 0xFFFF;
	idt[i].selector 	= 0x08;
	idt[i].zero 		= 0;
	idt[i].flags 		= handler ? flags : 0; /* mark not-present when handler is null */
}

void idt_init(void) {
	idt_ptr.limit = sizeof(idt) - 1;
	idt_ptr.base = (uint32_t) &idt;

	for (int i = 0; i < IDT_ENTRIES; i++) {
		idt_set_entry(i, 0, 0);
	}

	/* CPU exceptions */
	idt_set_entry(0,  (uint32_t) isr0,  0x8E);
	idt_set_entry(1,  (uint32_t) isr1,  0x8E);
	idt_set_entry(2,  (uint32_t) isr2,  0x8E);
	idt_set_entry(3,  (uint32_t) isr3,  0x8E);
	idt_set_entry(4,  (uint32_t) isr4,  0x8E);
	idt_set_entry(5,  (uint32_t) isr5,  0x8E);
	idt_set_entry(6,  (uint32_t) isr6,  0x8E);
	idt_set_entry(7,  (uint32_t) isr7,  0x8E);
	idt_set_entry(8,  (uint32_t) isr8,  0x8E);
	idt_set_entry(9,  (uint32_t) isr9,  0x8E);
	idt_set_entry(10, (uint32_t) isr10, 0x8E);
	idt_set_entry(11, (uint32_t) isr11, 0x8E);
	idt_set_entry(12, (uint32_t) isr12, 0x8E);
	idt_set_entry(13, (uint32_t) isr13, 0x8E);
	idt_set_entry(14, (uint32_t) isr14, 0x8E);
	idt_set_entry(15, (uint32_t) isr15, 0x8E);
	idt_set_entry(16, (uint32_t) isr16, 0x8E);
	idt_set_entry(17, (uint32_t) isr17, 0x8E);
	idt_set_entry(18, (uint32_t) isr18, 0x8E);
	idt_set_entry(19, (uint32_t) isr19, 0x8E);
	idt_set_entry(20, (uint32_t) isr20, 0x8E);
	idt_set_entry(21, (uint32_t) isr21, 0x8E);
	idt_set_entry(22, (uint32_t) isr22, 0x8E);
	idt_set_entry(23, (uint32_t) isr23, 0x8E);
	idt_set_entry(24, (uint32_t) isr24, 0x8E);
	idt_set_entry(25, (uint32_t) isr25, 0x8E);
	idt_set_entry(26, (uint32_t) isr26, 0x8E);
	idt_set_entry(27, (uint32_t) isr27, 0x8E);
	idt_set_entry(28, (uint32_t) isr28, 0x8E);
	idt_set_entry(29, (uint32_t) isr29, 0x8E);
	idt_set_entry(30, (uint32_t) isr30, 0x8E);
	idt_set_entry(31, (uint32_t) isr31, 0x8E);

	/* PIC IRQs 0-15 mapped to IDT 32-47 */
	idt_set_entry(32, (uint32_t) isr32, 0x8E);
	idt_set_entry(33, (uint32_t) isr33, 0x8E);
	idt_set_entry(34, (uint32_t) isr34, 0x8E);
	idt_set_entry(35, (uint32_t) isr35, 0x8E);
	idt_set_entry(36, (uint32_t) isr36, 0x8E);
	idt_set_entry(37, (uint32_t) isr37, 0x8E);
	idt_set_entry(38, (uint32_t) isr38, 0x8E);
	idt_set_entry(39, (uint32_t) isr39, 0x8E);
	idt_set_entry(40, (uint32_t) isr40, 0x8E);
	idt_set_entry(41, (uint32_t) isr41, 0x8E);
	idt_set_entry(42, (uint32_t) isr42, 0x8E);
	idt_set_entry(43, (uint32_t) isr43, 0x8E);
	idt_set_entry(44, (uint32_t) isr44, 0x8E);
	idt_set_entry(45, (uint32_t) isr45, 0x8E);
	idt_set_entry(46, (uint32_t) isr46, 0x8E);
	idt_set_entry(47, (uint32_t) isr47, 0x8E);


    idt_load((uint32_t) &idt_ptr);
}
