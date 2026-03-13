/* vga.c */

#include "../include/vga.h"

#define VGA_ADDRESS 0XB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t *vga_buffer = (uint16_t *) VGA_ADDRESS;
static uint8_t terminal_color;
static size_t cursor_row;
static size_t cursor_col;

/* Mounts VGA entry [color | char] as uint16_t */
static inline uint16_t vga_entry(char c, uint8_t color) {
	return (uint16_t) c | ((uint16_t) color << 8);
}

/* Mounts color byte from fg and bg */
static inline uint8_t vga_color(vga_color_t fg, vga_color_t bg) {
	return fg | (bg << 4);
}

void terminal_init(void) {
	cursor_row = 0;
	cursor_col = 0;
	terminal_color = vga_color(VGA_LIGHT_GREY, VGA_BLACK);
	terminal_clear();
}

void terminal_clear(void) {
	for (size_t row = 0; row < VGA_HEIGHT; row++) {
		for (size_t col = 0; col < VGA_WIDTH; col++) {
			vga_buffer[row * VGA_WIDTH + col] = vga_entry(' ', terminal_color);
		}
	}
	cursor_row = 0;
	cursor_col = 0;
}

void terminal_setcolor(vga_color_t fg, vga_color_t bg) {
	terminal_color = vga_color(fg, bg);
}

static void terminal_scroll(void) {
	for (size_t row = 1; row < VGA_HEIGHT; row++) {
		for (size_t col = 0; col < VGA_WIDTH; col++) {
			vga_buffer[(row - 1) * VGA_WIDTH + col] = vga_buffer[row * VGA_WIDTH + col];
		}
	}
	for (size_t col = 0; col < VGA_WIDTH; col++) {
		vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = vga_entry(' ', terminal_color);
	}
	cursor_row = VGA_HEIGHT - 1;
	cursor_col = 0;
}

void kputchar(char c) {
	if (c == '\n') {
		cursor_col = 0;
		cursor_row++;
	} else if (c == '\r') {
		cursor_col = 0;
	} else if (c == '\t') {
		cursor_col = (cursor_col + 4) & ~3;
	} else {
		vga_buffer[cursor_row * VGA_WIDTH + cursor_col] = vga_entry(c, terminal_color);
		cursor_col++;
	}

	if (cursor_col >= VGA_WIDTH) {
		cursor_col = 0;
		cursor_row++;
	}

	if (cursor_row >= VGA_HEIGHT) {
		terminal_scroll();
	}
}

void kprint(const char *str) {
	for (size_t i = 0; str[i] != '\0'; i++) {
		kputchar(str[i]);
	}
}

void kprint_uint(uint32_t n, uint8_t base) {
	static const char digits[] = "0123456789ABCDEF";
	char buff[32];
	int i = 0;

	if (n == 0) {
		kputchar('0');
		return;
	}

	while (n > 0) {
		buff[i++] = digits[n % base];
		n /= base;
	}

	while (i > 0) {
		kputchar(buff[--i]);
	}
}

void kprint_int(int32_t n) {
	if (n < 0) {
		kputchar('-');
		kprint_uint((uint32_t) -n, 10);
	} else {
		kprint_uint((uint32_t) n, 10);
	}
}

void kprint_hex(uint32_t n) {
	kprint("0x");
	kprint_uint(n, 16);
}
