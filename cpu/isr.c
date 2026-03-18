/* isr.c */

#include "../include/isr.h"
#include "../include/vga.h"
#include "../include/pic.h"

static const char *exception_messages[] = {
	"Division Error",
	"Debug",
	"Non-Maskable Interrupt",
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invalid Opcode",
	"Device Not Available",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection Fault",
	"Page Fault",
	"Reserved",
	"x87 Floating-Point Exception",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Point Exception",
	"Virtualization Exception",
	"Control Protection Exception",
	"Reserved", "Reserved", "Reserved", "Reserved",
	"Reserved", "Reserved", "Reserved", "Reserved",
	"Security Exception",
	"Reserved"
};

static irq_handler_t irq_handlers[16] = {0};

void irq_register(uint8_t irq, irq_handler_t handler) {
	irq_handlers[irq] = handler;
}

void isr_handler(registers_t *regs) {
	if (regs->int_no >= 32) {
		uint8_t irq = regs->int_no - 32;
		if (irq < 16 && irq_handlers[irq]) {
			irq_handlers[irq](regs);
		}
		pic_send_eoi(irq);
		return;
	}

	terminal_setcolor(VGA_LIGHT_RED, VGA_BLACK);
	kprint("\n[EXCEPTION] ");
	kprint(exception_messages[regs->int_no]);

	kprint("\n  int=");
	kprint_uint(regs->int_no, 10);

	kprint("  err=");
	kprint_hex(regs->err_code);

	kprint("\n  eip=");
	kprint_hex(regs->eip);

	kprint("  cs=");
	kprint_hex(regs->cs);

	kprint("\n  eax=");
	kprint_hex(regs->eax);

	kprint("  ebx=");
	kprint_hex(regs->ebx);

	terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);

	while (1) {};
}
