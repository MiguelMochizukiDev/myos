/* pic.c */

#include "cpu/pic.h"

static inline void outb(uint16_t port, uint8_t val) {
       	__asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	__asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

static inline void io_wait(void) {
	outb(0x80, 0);
}

void pic_remap(void) {
	/* Save current masks */
	uint8_t mask1 = inb(PIC1_DATA);
	uint8_t mask2 = inb(PIC2_DATA);

	/* ICW1 */
	outb(PIC1_CMD,  0x11); io_wait();
	outb(PIC2_CMD,  0x11); io_wait();

	/* ICW2: sets each PIC base vector */
	outb(PIC1_DATA, 0x20); io_wait(); /* IRQ 0-7  → vectors 32-39 */
	outb(PIC2_DATA, 0x28); io_wait(); /* IRQ 8-15 → vectors 40-47 */

	/* ICW3: sets master slave cascade */
	outb(PIC1_DATA, 0x04); io_wait(); /* master: slave on IRQ2 */
	outb(PIC2_DATA, 0x02); io_wait(); /* slave: identity = 2 */

	/* ICW4: 8086 mode */
	outb(PIC1_DATA, 0x01); io_wait();
	outb(PIC2_DATA, 0x01); io_wait();

	/* Restore masks */
	outb(PIC1_DATA, mask1);
	outb(PIC2_DATA, mask2);
}

void pic_send_eoi(uint8_t irq) {
	if (irq >= 8)
		outb(PIC2_CMD, PIC_EOI);
	outb(PIC1_CMD, PIC_EOI);
}

/* Mask all hardware IRQs — useful during initialization */
void pic_mask_all(void) {
	outb(PIC1_DATA, 0xFF);
	outb(PIC2_DATA, 0xFF);
}

void pic_unmask(uint8_t irq) {
	uint16_t port;
	uint8_t mask;

	if (irq < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		irq -= 8;
	}

	mask = inb(port) & ~(1 << irq);
	outb(port, mask);
}
