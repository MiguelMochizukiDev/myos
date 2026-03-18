/* timer.c */

#include "drivers/timer.h"
#include "cpu/isr.h"
#include "cpu/pic.h"

static uint32_t tick = 0;

uint32_t timer_get_tick(void) {
	return tick;
}

static void timer_handler(registers_t *regs) {
	(void) regs;
	tick++;
}

void timer_init(void) {
	irq_register(0, timer_handler);
	pic_unmask(0);
}
